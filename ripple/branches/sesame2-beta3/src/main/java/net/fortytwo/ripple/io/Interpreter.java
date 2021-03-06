package net.fortytwo.ripple.io;

import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.PrintStream;
import java.io.OutputStreamWriter;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

import jline.Completor;
import jline.MultiCompletor;
import jline.ConsoleReader;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.ast.ListAst;
import net.fortytwo.ripple.io.ContainerTreeView;
import net.fortytwo.ripple.model.ContainerSink;
import net.fortytwo.ripple.model.Dereferencer;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

/**
 * Console input:
 *     System.in --> reader --> readOut --> writeIn --> RippleLexer
 *
 * Normal output:
 *     [commands and queries] --> queryEngine.getPrintStream()
 *
 * Error output:
 *     alert() --> queryEngine.getErrorPrintStream()
 */
public class Interpreter extends Thread implements Observer
{
	private final static Logger s_logger
		= Logger.getLogger( Interpreter.class );

	private PipedInputStream  writeIn;
	private PipedOutputStream readOut;

	private ConsoleReader reader;
	private int lineNumber = 0;

	private QueryEngine queryEngine;

	private RecognizerInterface recognizerInterface;

	////////////////////////////////////////////////////////////////////////////

	public Interpreter( final QueryEngine qe, final InputStream is )
		throws RippleException
	{
		queryEngine = qe;

		String jLineDebugOutput = Ripple.getJLineDebugOutput();

		try
		{
			reader = new ConsoleReader( is,
				new OutputStreamWriter( qe.getPrintStream() ) );

			if ( null != jLineDebugOutput )
				reader.setDebug(
					new PrintWriter(
						new FileWriter( jLineDebugOutput, true ) ) );

			writeIn = new PipedInputStream();
			readOut = new PipedOutputStream( writeIn );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		queryEngine.getLexicon().addObserver( this );

		update( queryEngine.getLexicon(), null );

		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
				evaluate( ast );
			}
		};

		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd )
				throws RippleException
			{
				execute( cmd );
			}
		};

		Sink<RecognizerEvent> eventSink = new Sink<RecognizerEvent>()
		{
			public void put( final RecognizerEvent event )
				throws RippleException
			{
				switch ( event )
				{
					case NEWLINE:
						readLine();
						break;
					default:
						throw new RippleException( "event not yet supported: "
							+ event );
				}
			}
		};

		recognizerInterface = new RecognizerInterface(
			querySink, commandSink, eventSink, qe.getErrorPrintStream() );
	}

	////////////////////////////////////////////////////////////////////////////

	public void update( Observable o, Object arg )
	{
		if ( o instanceof Lexicon )
			updateCompletors( (Lexicon) o );
	}

	public void run()
	{
		try
		{
			s_logger.debug( "running Interpreter as a new thread" );
			runPrivate();
		}

		catch ( Throwable t )
		{
			alert( "Error: " + t.toString() );

			// Log the error.
			new RippleException( t );
		}
	}

	private void readLine()
	{
		try
		{
			++lineNumber;
			String line = reader.readLine( "" + lineNumber + " >>  " );
	
			if ( null != line )
			{
				byte[] bytes = line.getBytes();
				readOut.write( bytes, 0, bytes.length );
	
				// Add a deliberate "end of line" character so the lexer knows
				// to call readLine() again when it gets there.
				byte[] terminator = { '\n' };
				readOut.write( terminator, 0, 1 );
	
				readOut.flush();
			}
		}

		// This has never happened.
		catch ( java.io.IOException e )
		{
			alert( "IOException: " + e.toString() );
		}
	}

	private void execute( final Command cmd )
	{
		ModelConnection mc = null;
		boolean gotConnection = false, finished = false;

		try
		{
			mc = new ModelConnection( queryEngine.getModel(), "Command" );
			gotConnection = true;
			cmd.execute( queryEngine, mc );
			finished = true;
			mc.close();
		}

		catch ( ParserQuitException e )
		{
			try
			{
				mc.close();
			}

			catch ( RippleException e2 )
			{
				alert( "Error: failed to close connection" );
			}

			throw e;
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );

			if ( !gotConnection )
				alert( "Error: failed to establish connection" );

			else if ( !finished )
			{
				try
				{
					mc.close();
				}

				catch ( RippleException e2 )
				{
					alert( "Error: failed to close connection" );
				}
			}

			else
				alert( "Error: failed to close connection" );
		}
	}

	private void evaluate( ListAst ast )
	{
		ModelConnection mc = null;
		boolean gotConnection = false, finished = false;

		try
		{
			mc = new ModelConnection( queryEngine.getModel(), "for Interpreter evaluate()" );
			gotConnection = true;

			// Results are first dereferenced, then placed into a buffer which
			// will be flushed into the view after the lexicon is updated.
			ContainerTreeView view = new ContainerTreeView(
				queryEngine.getPrintStream(), mc );
			final Buffer<RippleList> buff = new Buffer<RippleList>( view );
			final ModelConnection mcf = mc;
			Sink<RippleList> derefSink = new Sink<RippleList>()
			{
				public void put( final RippleList list) throws RippleException
				{
					dereference( list.getFirst(), mcf );
					buff.put( list );
				}
			};

			RippleQueryCmd cmd = new RippleQueryCmd( ast, derefSink );

			queryEngine.getLexicon().suspendEventHandling();
			cmd.execute( queryEngine, mc );
			queryEngine.getLexicon().resumeEventHandling();

			// Flush results to the view.
			queryEngine.getPrintStream().println( "" );
			buff.flush();
			if ( view.size() > 0 )
				queryEngine.getPrintStream().println( "" );

			finished = true;
			mc.close();
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );

			if ( !gotConnection )
				alert( "Error: failed to establish connection" );

			else if ( !finished )
			{
				try
				{
					mc.close();
				}

				catch ( RippleException e2 )
				{
					alert( "Error: failed to close connection" );
				}
			}

			else
				alert( "Error: failed to close connection" );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private void dereference( RippleValue v, ModelConnection mc )
		throws RippleException
	{
		Dereferencer d = mc.getModel().getDereferencer();
		RdfValue r = v.toRdf( mc );

		try
		{
			d.dereference( r, mc );
		}

		catch ( RippleException e )
		{
			// (soft fail)
		}
	}

	private void updateCompletors( final Lexicon lexicon )
	{
		s_logger.debug( "updating completors" );
		List completors = new ArrayList();

		try
		{
			completors.add( lexicon.getCompletor() );

			ArrayList<String> directives = new ArrayList<String>();
			directives.add( "@count" );
			directives.add( "@define" );
			directives.add( "@export" );
			directives.add( "@help" );
			directives.add( "@list" );
			directives.add( "@prefix" );
			directives.add( "@quit" );
			directives.add( "@saveas" );
			directives.add( "@serql" );
			directives.add( "@undefine" );

			completors.add(
				new net.fortytwo.ripple.io.jline.DirectiveCompletor(
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

	private void clear( final InputStream is )
	{
		try
		{
			int lim = is.available();
			for ( int i = 0; i < lim; i++ )
				is.read();
		}

		catch ( java.io.IOException e )
		{
			alert( "Error: " + e );
		}
	}

	private void runPrivate()
	{
		// Break out when a @quit directive is encountered
		for (;;)
		{
			// If there's anything in the input buffer, it's because the parser
			// ran across a syntax error.  Clear the buffer and start afresh.
			clear( writeIn );

			// Get the first line of input (the parser itself will request
			// additional input when it's ready).
			readLine();

			RippleLexer lexer = new RippleLexer( writeIn );
			lexer.initialize( recognizerInterface );
			RippleParser parser = new RippleParser( lexer );
			parser.initialize( recognizerInterface );

			try
			{
				parser.nt_Document();

				// If the parser has exited normally, then we're done.
				break;
			}

			// Non-fatal.
			catch ( antlr.RecognitionException e )
			{
				alert( "RecognitionException: " + e.toString() );
			}

			// Non-fatal.
			catch ( antlr.TokenStreamException e )
			{
				alert( "TokenStreamException: " + e.toString() );
			}

			// This happens, for instance, when the parser receives a value
			// which is too large for the target data type.  Non-fatal.
			catch ( NumberFormatException e )
			{
				alert( e.toString() );
			}

			// The parser has received a quit command.
			catch ( ParserQuitException e )
			{
				s_logger.debug( "quit() called on Interpreter" );

				break;
			}
		}
	}

	private void alert( String s )
	{
		queryEngine.getErrorPrintStream().println( "\n" + s + "\n" );
	}
}

// kate: tab-width 4
