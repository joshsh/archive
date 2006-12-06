package wurfel.cli;

import wurfel.Context;
import wurfel.WurfelException;
//import wurfel.model.Node;
import wurfel.Node;

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

import java.net.URL;
import java.net.MalformedURLException;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

public class Interpreter extends Thread implements Runnable
{
    private Context context;

    private PipedInputStream  writeIn;
    private PipedOutputStream readOut;

    private PrintWriter out;

    private ConsoleReader reader;
    private int lineNumber = 0;

    private CompletorState completorState = CompletorState.NONE;

    public void setCompletorState( final CompletorState state )
    {
        completorState = state;

        List completors = new ArrayList();

        Completor modelCompletor = context.getModel().getCompletor();
        completors.add( modelCompletor );

        SimpleCompletor commandCompletor = new SimpleCompletor( new String [] {
            "!count",
            "!define",
            "!import",
            "!print",
            "!resolve",
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
    }

    public Interpreter( Context context ) throws WurfelException
    {
        this.context = context;

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
                bytes[0] = '\n';
                readOut.write( bytes, 0, 1 );

                out.flush();
            }

            return true;
        }

        catch( Throwable t )
        {
            System.err.println( t.toString() );
            return false;
        }
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

    public int countStatements()
    {
        return context.countStatements();
    }

    public void define( final String name, final String uri )
    {
        context.define( name, uri );
    }

    public String resolve( final String name )
    {
        return context.resolve( name );
    }

    public void show( final String name )
    {
        try
        {
            context.show( name );
        }

        catch ( WurfelException e )
        {
            System.err.println( e.getMessage() );
        }
    }

    public void importModel( final String urlStr, final String baseURI )
    {
        try
        {
            URL url = new URL( urlStr );
            context.importModel( url, baseURI );
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

    public void printStatements()
    {
        try
        {
            context.printStatements();
        }

        catch ( WurfelException e )
        {
            System.err.println( e.getMessage() );
        }
    }












    public void evaluate( Node r )
    {
        try
        {
// TODO: the printing should be done here, rather than in the Context.
            context.evaluate( r );
        }

        catch ( WurfelException e )
        {
            System.err.println( e.getMessage() );
        }
//        System.out.println( r.toString() );
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
