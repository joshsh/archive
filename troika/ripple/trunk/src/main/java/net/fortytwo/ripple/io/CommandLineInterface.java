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

import jline.Completor;
import jline.MultiCompletor;
import jline.ConsoleReader;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.ast.ListAst;
import net.fortytwo.ripple.io.ContainerTreeView;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.RdfValue;
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
public class CommandLineInterface extends Thread
{
	final static Logger s_logger
		= Logger.getLogger( CommandLineInterface.class );

	PipedInputStream  writeIn;
	PipedOutputStream readOut;

	Interpreter interpreter;

	ConsoleReader reader;
	int lineNumber = 0;

	QueryEngine queryEngine;

	////////////////////////////////////////////////////////////////////////////

	public CommandLineInterface( final QueryEngine qe, final InputStream is )
		throws RippleException
	{
		queryEngine = qe;

		String jLineDebugOutput = Ripple.getJLineDebugOutput();

		try
		{
			reader = new ConsoleReader( is,
				new OutputStreamWriter( qe.getPrintStream() ) );

			// Set up JLine logging if asked for.
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

		// Initialize completors.
		updateCompletors();

		// What do we do with queries?
		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
				evaluate( ast );
			}
		};

		// What do we do with commands?
		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd )
				throws RippleException
			{
				execute( cmd );
			}
		};

		// What do we do with parser events?
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
					case ESCAPE:
System.out.println( "Escape!" );
						break;
					default:
						throw new RippleException( "event not yet supported: "
							+ event );
				}
			}
		};

		RecognizerInterface itf = new RecognizerInterface(
			querySink, commandSink, eventSink, qe.getErrorPrintStream() );

		Sink<Exception> parserExceptionSink = new Sink<Exception>()
		{
			public void put( final Exception e )
				throws RippleException
			{
				// This happens, for instance, when the parser receives a value
				// which is too large for the target data type.  Non-fatal.
				if ( e instanceof NumberFormatException )
					alert( e.toString() );

				// Non-fatal.
				else if ( e instanceof antlr.RecognitionException )
					alert( "RecognitionException: " + e.toString() );

				// Non-fatal.
				else if ( e instanceof antlr.TokenStreamException )
					alert( "TokenStreamException: " + e.toString() );

				else
					throw new RippleException(
						"non-recoverable exception thrown: " + e.toString() );
			}
		};

		interpreter = new Interpreter( itf, writeIn, parserExceptionSink );
	}

	public void run()
	{
		try
		{
			s_logger.debug( "running CommandLineInterface as a new thread" );
			interpreter.parse();
		}

		catch ( Throwable t )
		{
			alert( "Error: " + t.toString() );

			// Log the error.
			new RippleException( t );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void readLine()
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

	void execute( final Command cmd )
	{
		ModelConnection mc = null;
		boolean gotConnection = false, finished = false;

		try
		{
			mc = queryEngine.getConnection( "Command" );
			gotConnection = true;
			cmd.execute( queryEngine, mc );
			finished = true;
			mc.close();

			// Model may have changed, so update completors.
			updateCompletors();
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

	void evaluate( ListAst ast )
	{
		ModelConnection mc = null;
		boolean gotConnection = false, finished = false;
		boolean doBuffer = Ripple.getBufferTreeView();

		try
		{
			queryEngine.getPrintStream().println( "" );

			mc = queryEngine.getConnection( "for CommandLineInterface evaluate()" );
			gotConnection = true;

			// Results are first dereferenced, then placed into a buffer which
			// will be flushed into the view after the lexicon is updated.
			ContainerTreeView view = new ContainerTreeView(
				queryEngine.getPrintStream(), mc );
			final Sink<RippleList> results = doBuffer
				? new Buffer<RippleList>( view )
				: view;

			final ModelConnection mcf = mc;
			Sink<RippleList> derefSink = new Sink<RippleList>()
			{
				public void put( final RippleList list) throws RippleException
				{
					dereference( list.getFirst(), mcf );
					results.put( list );
				}
			};

			RippleQueryCmd cmd = new RippleQueryCmd( ast, derefSink );

			cmd.execute( queryEngine, mc );

			// Flush results to the view.
			if ( doBuffer )
				( (Buffer<RippleList>) results ).flush();

			if ( view.size() > 0 )
				queryEngine.getPrintStream().println( "" );

			finished = true;
			mc.close();

			// Model may have changed, so update completors.
			updateCompletors();
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

	void dereference( RippleValue v, ModelConnection mc )
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

	void updateCompletors()
	{
		s_logger.debug( "updating completors" );
		List completors = new ArrayList();

		try
		{
			completors.add( queryEngine.getLexicon().getCompletor() );

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

	void alert( String s )
	{
		queryEngine.getErrorPrintStream().println( "\n" + s + "\n" );
	}
}

// kate: tab-width 4
