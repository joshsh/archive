package net.fortytwo.ripple.cli;

import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;

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
import net.fortytwo.ripple.model.LazyEvaluator;
import net.fortytwo.ripple.model.EagerStackEvaluator;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.Model;
//import net.fortytwo.ripple.model.DebugEvaluator;
import net.fortytwo.ripple.model.ObservableContainer;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.ListContainerSink;
import net.fortytwo.ripple.model.ListNode;
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
                evaluator = new LazyEvaluator();
                break;

            case COMPOSITIONAL:
                evaluator = new EagerStackEvaluator();
                break;
        }
//        evaluator = new DebugEvaluator( new LazyEvaluator( model ) );
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

        catch ( IOException e )
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

            try
            {
                SimpleCompletor commandCompletor = new SimpleCompletor( new String [] {
                    "@add",
                    "@count",
                    "@export",
                    "@list",
                    "@prefix",
                    "@quit",
                    "@serql",
                    "@term" } );
                completors.add( commandCompletor );

                Completor fileNameCompletor = new FileNameCompletor();
                completors.add( fileNameCompletor );

                // This makes candidates from multiple completors available at once.
                Completor multiCompletor = new MultiCompletor( completors );

                // This allows the user to complete an expression even when it is not
                // the first whitespace-delimited item on the current line.
                Completor argumentCompletor = new ArgumentCompletor( multiCompletor );

                Collection<Completor> existingCompletors = reader.getCompletors();
                if ( existingCompletors.size() > 0 )
                    reader.removeCompletor( existingCompletors.iterator().next() );

                reader.addCompletor( argumentCompletor );
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

    public void define( final String name, final String uri )
    {
        model.define( name, uri );
    }

    public void showContexts()
    {
        try
        {
//            valueSet.setValues( model.getContexts() );

// FIXME: this is a kludge to keep the print stream from using namespace prefixes instead of full URI references
            printStream.println( "" );
            Iterator<Value> models = model.getContexts().iterator();
            int i = 0;
            while ( models.hasNext() )
            {
                printStream.print( "[" + i++ + "] " );
                printStream.println( models.next().toString() );
            }
            printStream.println( "" );
        }

        catch ( RippleException e ) {}
    }

    public void saveAs( final String fileName )
    {
        try
        {
            model.saveAs( fileName );

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

            Value subjValue = subj.evaluate( this, mc );
            Value predValue = pred.evaluate( this, mc );
            Value objValue = obj.evaluate( this, mc );

            mc.addStatement( subjValue, predValue, objValue );
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

            URI ns = (URI) uri.evaluate( this, mc );
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

    private void dereferenceResultSet( Collection<Value> values, ModelConnection mc )
        throws RippleException
    {
        Dereferencer d = model.getDereferencer();

        Iterator<Value> iter = values.iterator();
        while ( iter.hasNext() )
        {
            Value value = iter.next();
if ( value instanceof ListNode )
value = ( (net.fortytwo.ripple.model.ListNode<Value>) value ).getFirst();

            if ( value instanceof URI )
            {
                try
                {
                    d.dereference( (URI) value, mc );
                }

                catch ( RippleException e )
                {
                    // (soft fail)
                }
            }
        }
    }

    public Value resolveUnqualifiedName( final String localName )
        throws RippleException
    {
        List<URI> options = lexicon.resolveUnqualifiedName( localName );
        if ( null == options || 0 == options.size() )
            return null;
        else
        {
// TODO: this is only one way of handling name ambiguity.
            Value choice = options.get( 0 );

            return model.translateFromGraph( choice );
        }
    }

    public Value resolveQualifiedName( final String nsPrefix,
                                       final String localName )
        throws RippleException
    {
        Value v = lexicon.resolveQualifiedName( nsPrefix, localName );

        return ( null == v )
            ? null
            : model.translateFromGraph( v );
    }

    private Container reduce( Value expr, ModelConnection mc )
        throws RippleException
    {
        try
        {
            ListContainerSink sink = new ListContainerSink();
// FIXME: awkward
            if ( expr instanceof ListNode )
                evaluator.reduce( (ListNode<Value>) expr, sink, mc );
            else
                evaluator.reduce( new ListNode<Value>( expr ), sink, mc );

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

            Value expr = ast.evaluate( this, mc );

            Container result = ( null == expr )
                ? new Container()
                : reduce( expr, mc );

// TODO: this should dereference as many levels as Ripple.getTreeViewDepth(),
//       and should probably be moved into the tree view itself if possible.
            dereferenceResultSet( result, mc );

            if ( null != name )
            {
                if ( 0 == result.size() )
                    throw new RippleException( "null value in assignment" );

                else if ( 1 < result.size() )
                    throw new RippleException( "ambiguous value in assigment" );

                else
                {
                    Value srcVal = result.iterator().next();
                    String defaultNs = lexicon.resolveNamespacePrefix( "" );

                    if ( null == defaultNs )
                        throw new RippleException( "no default namespace is defined" );

// TODO: check for collision with an existing URI

                    Value destVal = mc.toRdf( srcVal );

// TODO
                    URI uri = mc.createUri( defaultNs, name );
URI owlSameAsUri = mc.createUri( "http://www.w3.org/2002/07/owl#sameAs" );
                    mc.add( uri, owlSameAsUri, destVal );
                }

                lexicon.update();
            }

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
