package net.fortytwo.ripple.cli;

import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.PrintStream;
import java.io.OutputStream;
import java.io.FileOutputStream;

import java.net.URL;
import java.net.MalformedURLException;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.Observable;
import java.util.Observer;

import jline.Completor;
import jline.FileNameCompletor;
import jline.ArgumentCompletor;
import jline.SimpleCompletor;
import jline.MultiCompletor;
import jline.ConsoleReader;

import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Value;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.Dereferencer;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Evaluator;
import net.fortytwo.ripple.model.EagerStackEvaluator;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.Model;
//import net.fortytwo.ripple.model.DebugEvaluator;
import net.fortytwo.ripple.model.ObservableContainer;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.ListContainerSink;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.cli.ast.Ast;
import net.fortytwo.ripple.cli.ast.UriNode;

import org.apache.log4j.Logger;

/**
 *  Console input:
 *    System.in --> reader --> readOut --> writeIn --> lexer
 *
 *  Normal output:
 *    valueSetObserver --> printStream --> System.out
 *    evaluateGraphQuery() --> printStream --> System.out
 *
 *  Error output:
 *    alert() --> errorPrintStream = System.err
 */
public class Interpreter extends Thread implements Observer
{
    private final static Logger s_logger
        = Logger.getLogger( Interpreter.class );

    private Model model;
    private Evaluator evaluator;

public Model getModel()
{
    return model;
}

    private PipedInputStream  writeIn;
    private PipedOutputStream readOut;

    private PrintWriter out;

    private ConsoleReader reader;
    private int lineNumber = 0;

    private RipplePrintStream printStream;
    private PrintStream errorPrintStream;

    private CompletorState completorState = CompletorState.NONE;

    private ObservableContainer valueSet;
    private ContainerTreeView valueSetObserver;

    private Lexicon lexicon;

    ////////////////////////////////////////////////////////////////////////////

    // A helper variable for the lexer and parser.
    private String languageTag;

    public String getLanguageTag()
    {
        return languageTag;
    }

    public void setLanguageTag( final String tag )
    {
        languageTag = tag;
    }

    private void chooseEvaluator()
    {
        switch ( Ripple.getEvaluationStyle() )
        {
            case APPLICATIVE:
// TODO: not implemented
                break;

            case COMPOSITIONAL:
                evaluator = new EagerStackEvaluator();
                break;
        }
//        evaluator = new DebugEvaluator( new EagerStackEvaluator( model ) );
    }

    ////////////////////////////////////////////////////////////////////////////

    public Interpreter( Model model ) throws RippleException
    {
        this.model = model;

        chooseEvaluator();

        lexicon = new Lexicon( model );
        lexicon.addObserver( this );

        String jLineDebugOutput = Ripple.getJLineDebugOutput();

        try
        {
            reader = new ConsoleReader();

            if ( null != jLineDebugOutput )
                reader.setDebug(
                    new PrintWriter(
                        new FileWriter( jLineDebugOutput, true ) ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        try
        {
            writeIn = new PipedInputStream();
            readOut = new PipedOutputStream( writeIn );

            out = new PrintWriter( System.out );
        }

        catch ( java.io.IOException e )
        {
            throw new RippleException( e );
        }

        errorPrintStream = System.err;

        valueSet = new ObservableContainer( model, null );
ModelConnection mc = new ModelConnection( model, "for ConsoleValueSet constructor" );
        printStream = new RipplePrintStream( System.out, lexicon, mc );
        valueSetObserver = new ContainerTreeView( valueSet, printStream );
mc.close();
        valueSet.addObserver( valueSetObserver );

        update( lexicon, null );
    }

    ////////////////////////////////////////////////////////////////////////////

    public void updateCompletors( final CompletorState state )
    {
System.out.println( "########## updating completors" );
        completorState = state;

        List completors = new ArrayList();

        try
        {
            Completor modelCompletor = lexicon.getCompletor();
            completors.add( modelCompletor );

            ArrayList<String> directives = new ArrayList<String>();
            directives.add( "@assert" );
            directives.add( "@count" );
            directives.add( "@export" );
            directives.add( "@list" );
            directives.add( "@prefix" );
            directives.add( "@quit" );
            directives.add( "@serql" );
            directives.add( "@term" );

            completors.add(
                new net.fortytwo.ripple.cli.jline.DirectiveCompletor(
                    directives ) );

            try
            {
                // This makes candidates from multiple completors available at once.
                Completor multiCompletor = new MultiCompletor( completors );

reader.addCompletor( multiCompletor );
            }

            catch ( Throwable t )
            {
                throw new RippleException( t );
            }
        }

        catch ( RippleException e )
        {
            s_logger.error( "Failed to update completors.  Continuing nonetheless." );
        }
    }

    public boolean readLine()
    {
        try
        {
            ++lineNumber;
            String line = reader.readLine( "" + lineNumber + " >>  " );

            if ( null == line )
                return false;

            else
            {
                byte[] bytes = line.getBytes();
                readOut.write( bytes, 0, bytes.length );

                // Add a deliberate "end of line" character so the lexer knows
                // to call readLine() again when it gets there.
                byte[] terminator = { '\n' };
                readOut.write( terminator, 0, 1 );

                out.flush();
            }

            return true;
        }

        catch( java.io.IOException e )
        {
            new RippleException( e );
            return false;
        }
    }

    public void quit()
    {
        s_logger.debug( "quit() called on Interpreter" );
        throw new ParserQuitException();
    }

    private void runPrivate() throws Throwable
    {
        if ( !readLine() )
            return;

// FIXME: this appears to work, but I need to go back and make sure it will ALWAYS work.
        while ( true )
        {
System.out.println( "--- 1 ---" );
            // NOTE: I didn't find a "reset" method in the generated lexer or
            // parser, so I'm assuming that we need to create a new lexer and
            // parser instance once a parsing error puts them in a weird
            // state (unless we want to go to the trouble of recovering from
            // it, at any rate).
            RippleLexer lexer = new RippleLexer( writeIn );
            lexer.initialize( this );
            RippleParser parser = new RippleParser( lexer );
            parser.initialize( this );
System.out.println( "--- 2 ---" );

            try
            {
                parser.nt_Input();

                // If the parser has exited normally, then we're done.
                break;
            }

            catch ( antlr.RecognitionException e )
            {
                // Report the error, then begin parsing again.
                alert( "RecognitionException: " + e.toString() );
            }

            catch ( antlr.TokenStreamRecognitionException e )
            {
                // Report the error, then begin parsing again.
                alert( "TokenStreamRecognitionException: " + e.toString() );
            }

            catch ( ParserQuitException e )
            {
                // The user has instructed the parser to quit.
                break;
            }
System.out.println( "--- 3 ---" );
        }
    }

    public void run()
    {
        try
        {
            s_logger.debug( "running Interpreter in a new thread" );
            runPrivate();
        }

        catch ( Throwable t )
        {
            alert( "Error: " + t.toString() );

            new RippleException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public void countStatements()
    {
        try
        {
            System.out.println( "\n" + model.countStatements() + "\n" );
        }

        catch ( RippleException e ) {}
    }

    public void showContexts()
    {
        try
        {
//            valueSet.setValues( model.getContexts() );

// FIXME: this is a kludge to keep the print stream from using namespace prefixes instead of full URI references
            printStream.println( "" );
            Iterator<RippleValue> models = model.getContexts().iterator();
            int i = 0;
            while ( models.hasNext() )
            {
                printStream.print( "[" + i++ + "] " );
                printStream.println( models.next() );
            }
            printStream.println( "" );
        }

        catch ( RippleException e ) {}
    }

    private void saveAsPrivate( String fileName )
        throws RippleException
    {
        OutputStream out;

        try
        {
            out = new FileOutputStream( fileName );
        }

        catch ( java.io.FileNotFoundException e )
        {
            throw new RippleException( e );
        }

        model.writeTrix( out );

        try
        {
            out.close();
        }

        catch ( java.io.IOException e )
        {
            throw new RippleException( e );
        }
    }

    public void saveAs( final String fileName )
    {
        try
        {
            saveAsPrivate( fileName );

            System.out.println( "\nSaved data set as '" + fileName + "'\n" );
        }

        catch ( RippleException e ) {}
    }

    public void addStatement( Ast subj, Ast pred, Ast obj )
    {
        ModelConnection mc = null;

        try
        {
            mc = new ModelConnection( model, "for addStatement" );

            RippleValue subjValue = subj.evaluate( this, mc );
            RippleValue predValue = pred.evaluate( this, mc );
            RippleValue objValue = obj.evaluate( this, mc );

            mc.add( subjValue, predValue, objValue );
            mc.close();
            mc = null;
        }

        catch ( RippleException e )
        {
            if ( null != mc )
            {
                try
                {
                    mc.close();
                }

                catch ( RippleException e2 )
                {
                    // ...
                }
            }

            alert( "Error: " + e.getMessage() );
        }
    }

    public void setNamespace( final String prefix, final UriNode uri )
    {
        ModelConnection mc = null;

        try
        {
            mc = new ModelConnection( model, "for setNamespace" );

            URI ns = mc.uriValue( uri.evaluate( this, mc ) );
            mc.setNamespace( prefix, ns );
            mc.close();
            mc = null;

            lexicon.update();
        }

        catch ( RippleException e )
        {
            if ( null != mc )
            {
                try
                {
                    mc.close();
                }

                catch ( RippleException e2 )
                {
                    // ...
                }
            }

            alert( "Error: " + e.getMessage() );
        }
    }

    private void dereferenceResultSet( Collection<RippleValue> values, ModelConnection mc )
        throws RippleException
    {
        Dereferencer d = model.getDereferencer();

        Iterator<RippleValue> iter = values.iterator();
        while ( iter.hasNext() )
        {
            RippleValue value = iter.next();
if ( value instanceof RippleList )
value = ( (net.fortytwo.ripple.model.RippleList) value ).getFirst();

            try
            {
                d.dereference( value.toRdf( mc ), mc );
            }

            catch ( RippleException e )
            {
                // (soft fail)
            }
        }
    }

    public RippleValue resolveUnqualifiedName( final String localName )
        throws RippleException
    {
        List<URI> options = lexicon.resolveUnqualifiedName( localName );
        if ( null == options || 0 == options.size() )
            return null;
        else
        {
// TODO: this is only one way of handling name ambiguity.
            RdfValue choice = new RdfValue( options.get( 0 ) );

            RippleValue rv = model.getBridge().get( choice );
            return ( null == rv )
                ? choice
                : rv;
        }
    }

    public RippleValue resolveQualifiedName( final String nsPrefix,
                                       final String localName )
        throws RippleException
    {
        Value v = lexicon.resolveQualifiedName( nsPrefix, localName );
        if ( null == v )
            return null;

        else
        {
            RdfValue choice = new RdfValue( v );

            RippleValue rv = model.getBridge().get( choice );
            return ( null == rv )
                ? choice
                : rv;
        }
    }

    private Container reduce( RippleValue expr, ModelConnection mc )
        throws RippleException
    {
        try
        {
            ListContainerSink sink = new ListContainerSink();
// FIXME: awkward

            RippleList list = ( expr instanceof RippleList )
                ? (RippleList) expr
                : new RippleList( expr );

                evaluator.applyTo( list, sink, mc );

            return sink;
        }

        catch ( RippleException e )
        {
            throw e;
        }
    }

    private void evaluate( Ast ast, final String name )
        throws RippleException
    {
        ModelConnection mc = null;

        try
        {
            mc = new ModelConnection( model, "for Interpreter evaluate()" );

            RippleValue expr = ast.evaluate( this, mc );

            // Define the term *before* reduction.
            if ( null != name )
            {
                String defaultNs = lexicon.resolveNamespacePrefix( "" );

                if ( null == defaultNs )
                    throw new RippleException( "no default namespace is defined" );

                if ( !( expr instanceof RippleList ) )
                    throw new RippleException( "term assignment for non-lists is not implemented" );

                RippleList exprList = (RippleList) expr;

// TODO: check for collision with an existing URI
                URI uri = mc.createUri( defaultNs + name );

                mc.copyStatements( exprList.toRdf( mc ), new RdfValue( uri ) );

                lexicon.update();
            }

            Container result = ( null == expr )
                ? new Container()
                : reduce( expr, mc );

// TODO: this should dereference as many levels as Ripple.getTreeViewDepth(),
//       and should probably be moved into the tree view itself if possible.
            dereferenceResultSet( result, mc );

            valueSet.setValues( result );

            mc.close();
        }

        catch ( RippleException e )
        {
            mc.close();
            throw e;
        }
    }

    public void evaluate( Ast ast )
    {
        ModelConnection mc = null;

        try
        {
            lexicon.suspendEventHandling();
            valueSetObserver.suspendEventHandling();
            evaluate( ast, null );
            lexicon.resumeEventHandling();
            valueSetObserver.resumeEventHandling();
        }

        catch ( RippleException e )
        {
            alert( "Error: " + e.getMessage() );
        }
    }

    public void evaluateAndDefine( Ast ast, String name )
    {
        ModelConnection mc = null;

        try
        {
            lexicon.suspendEventHandling();
            evaluate( ast, name );
            lexicon.resumeEventHandling();
        }

        catch ( RippleException e )
        {
            alert( "Error: " + e.getMessage() );
        }
    }

    public void showNamespaces()
    {
        try
        {
            model.showNamespaces();
        }

        catch ( RippleException e )
        {
            alert( "Error: " + e.getMessage() );
        }
    }

    // E.g.
    //      CONSTRUCT * FROM {x} p {y}
    public void evaluateGraphQuery( final String query )
    {
        ModelConnection mc = null;

        try
        {
            mc = new ModelConnection( model, "for evaluateGraphQuery" );
            Iterator<Statement> stmtIter = model.graphQuery( query ).iterator();

            printStream.println( "" );
            printStream.print( stmtIter );
            printStream.println( "" );

            mc.close();
        }

        catch ( RippleException e )
        {
            try
            {
                mc.close();
            }

            catch ( RippleException e2 )
            {
                // ...
            }

            alert( "Error: " + e.getMessage() );
        }
    }

    private void alert( String s )
    {
        errorPrintStream.println( "\n" + s + "\n" );
    }

    ////////////////////////////////////////////////////////////////////////////

    public void update( Observable o, Object arg )
    {
        if ( o == lexicon )
            updateCompletors( CompletorState.NONE );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
