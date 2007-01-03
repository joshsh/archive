package wurfel.cli;

import wurfel.Context;
import wurfel.WurfelException;
import wurfel.model.Evaluator;
import wurfel.model.LazyEvaluator;
//import wurfel.model.DebugEvaluator;
import wurfel.model.ObservableValueSet;
import wurfel.model.WurfelPrintStream;
import wurfel.cli.ast.Ast;

import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Value;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

import jline.Completor;
import jline.FileNameCompletor;
import jline.ArgumentCompletor;
import jline.SimpleCompletor;
import jline.MultiCompletor;
import jline.ConsoleReader;

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

import org.apache.log4j.Logger;

public class Interpreter extends Thread implements Runnable
{
    private final static Logger s_logger
        = Logger.getLogger( Interpreter.class );

    private Context context;
    private Evaluator evaluator;

    private PipedInputStream  writeIn;
    private PipedOutputStream readOut;

    private PrintWriter out;

    private ConsoleReader reader;
    private int lineNumber = 0;

    private CompletorState completorState = CompletorState.NONE;

    private ObservableValueSet valueSet;

    public void setCompletorState( final CompletorState state )
    {
        completorState = state;

        List completors = new ArrayList();

        try
        {
            Completor modelCompletor = context.getModel().getCompletor();
            completors.add( modelCompletor );
        }

        catch ( WurfelException e )
        {
            return;
        }

        SimpleCompletor commandCompletor = new SimpleCompletor( new String [] {
            "!add",
            "!count",
            "!define",
            "!graphQuery",
            "!import",
            "!namespaces",
            "!print",
            "!saveas",
            "!quit" } );
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

        valueSet = new ObservableValueSet( context, null );
        ConsoleValueSetObserver observer = new ConsoleValueSetObserver( valueSet );
        valueSet.addObserver( observer );
    }

    public Interpreter( Context context ) throws WurfelException
    {
        this.context = context;
        evaluator = new LazyEvaluator( context );
//        evaluator = new DebugEvaluator( new LazyEvaluator( context ) );

        try
        {
            reader = new ConsoleReader();
            reader.setDebug( new PrintWriter( new FileWriter("writer.debug", true ) ) );

            setCompletorState( CompletorState.COMMAND );

            writeIn = new PipedInputStream();
            readOut = new PipedOutputStream( writeIn );

            out = new PrintWriter( System.out );
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
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

        catch( Throwable t )
        {
            new WurfelException( t );
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
        ConsoleReader reader = new ConsoleReader();

        if ( !readLine() )
            return;

// FIXME: this appears to work, but I need to go back and make sure it will ALWAYS work.
        while ( true )
        {
            // NOTE: I didn't find a "reset" method in the generated lexer or
            // parser, so I'm assuming that we need to create a new lexer and
            // parser instance once a parsing error puts them in a weird
            // state (unless we want to go to the trouble of recovering from
            // it, any any rate).
            WurfelLexer lexer = new WurfelLexer( writeIn );
            lexer.initialize( this );
            WurfelParser parser = new WurfelParser( lexer );
            parser.initialize( this );

            try
            {
                parser.nt_Input();

                // If the parser has exited normally, then we're done.
                break;
            }

            catch ( antlr.RecognitionException e )
            {
                // Report the error, then begin parsing again.
                System.err.println( "Parse error: " + e.toString() );
            }

            catch ( antlr.TokenStreamRecognitionException e )
            {
                System.err.println( "Parse error: " + e.toString() );
            }

            catch ( ParserQuitException e )
            {
                break;
            }
        }
    }

    public void run()
    {
        try
        {
            runPrivate();
        }

        catch ( Throwable t )
        {
            System.err.println( t.toString() );

            new WurfelException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public void countStatements()
    {
        try
        {
            System.out.println( "\n" + context.countStatements() + "\n" );
        }

        catch ( WurfelException e ) {}
    }

    public void define( final String name, final String uri )
    {
        context.define( name, uri );
    }

    public void showContextIds()
    {
        try
        {
            context.showContextIds();
        }

        catch ( WurfelException e ) {}
    }

    public void importModel( final String urlStr, final String baseURI )
    {
        try
        {
            URL url = new URL( urlStr );
            if ( null == baseURI || 0 == baseURI.length() )
                context.importModel( url, null );
            else
                context.importModel( url, context.createUri( baseURI ) );
        }

        catch ( WurfelException e )
        {
            System.err.println( e.getMessage() );
        }

        catch ( MalformedURLException e )
        {
            System.err.println( e.getMessage() );
        }

        setCompletorState( CompletorState.COMMAND );
    }

    public void saveAs( final String fileName )
    {
        try
        {
            context.saveAs( fileName );

            System.out.println( "\nSaved data set as '" + fileName + "'\n" );
        }

        catch ( WurfelException e ) {}
    }

    public void addStatement( Ast subj, Ast pred, Ast obj )
    {
        try
        {
            Value subjValue = subj.evaluate( context );
            Value predValue = pred.evaluate( context );
            Value objValue = obj.evaluate( context );

            context.addStatement( subjValue, predValue, objValue );
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }

    public void evaluate( Ast ast )
    {
        try
        {
            Value expr = ast.evaluate( context );

            Collection<Value> result = ( null == expr )
                ? new ArrayList<Value>()
//                : context.reduce( expr );
                : evaluator.reduce( expr );

            valueSet.setValues( result );
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }

    public void showNamespaces()
    {
        try
        {
            context.showNamespaces();
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }

    // E.g.
    //      CONSTRUCT * FROM {x} p {y}
    public void evaluateGraphQuery( final String query )
    {
        try
        {
            WurfelPrintStream p = new WurfelPrintStream( System.out, context.getModel() );

            Iterator<Statement> stmtIter = context.graphQuery( query ).iterator();

            p.println( "" );
            p.print( stmtIter );
            p.println( "" );

//            p.close();
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
