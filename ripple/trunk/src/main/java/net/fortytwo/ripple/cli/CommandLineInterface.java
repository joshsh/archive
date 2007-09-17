/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.OutputStreamWriter;

import java.util.List;
import java.util.ArrayList;

import jline.Completor;
import jline.MultiCompletor;
import jline.ConsoleReader;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.cli.jline.DirectiveCompletor;
import net.fortytwo.ripple.control.Scheduler;
import net.fortytwo.ripple.control.TaskQueue;
import net.fortytwo.ripple.io.ThreadedInputStream;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.CollectorHistory;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

/**
 * A command-line interpreter/browser which coordinates user interaction with a Ripple query engine.
 */
public class CommandLineInterface
{
	private static final Logger LOGGER
		= Logger.getLogger( CommandLineInterface.class );

	private static final byte[] EOL = { '\n' };

	private PipedInputStream  writeIn;
	private PipedOutputStream readOut;
	private ThreadedInputStream consoleReaderInput;

	private Interpreter interpreter;

	private ConsoleReader reader;
	private int lineNumber;

	private QueryEngine queryEngine;

	private CollectorHistory<RippleList> queryResultHistory
		= new CollectorHistory<RippleList>( 2 );
private boolean lastQueryContinued = false;

	private TaskQueue taskQueue = new TaskQueue();

	////////////////////////////////////////////////////////////////////////////

	/**
	 * Console input:
	 *     is --> filter --> consoleReaderInput --> reader --> readOut --> writeIn --> interpreter
	 *
	 * Normal output:
	 *     [commands and queries] --> queryEngine.getPrintStream()
	 *
	 * Error output:
	 *     alert() --> queryEngine.getErrorPrintStream()
	 */
	public CommandLineInterface( final QueryEngine qe, final InputStream is )
		throws RippleException
	{
		queryEngine = qe;

		// Handling of queries
		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast ) throws RippleException
			{
				addCommand( new VisibleQueryCommand( ast, queryResultHistory, lastQueryContinued ) );
				lastQueryContinued = false;
				addCommand( new UpdateCompletorsCmd() );
				executeCommands();
			}
		};

		// Handling of "continuing" queries
		Sink<ListAst> continuingQuerySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast ) throws RippleException
			{
				addCommand( new VisibleQueryCommand( ast, queryResultHistory, lastQueryContinued ) );
				lastQueryContinued = true;
				addCommand( new UpdateCompletorsCmd() );
				executeCommands();
			}
		};

		// Handling of commands
		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd ) throws RippleException
			{
				addCommand( cmd );
				addCommand( new UpdateCompletorsCmd() );
				executeCommands();
			}
		};

		// Handling of parser events
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
						LOGGER.debug( "received escape event" );
						abortCommands();
						break;
					case QUIT:
						LOGGER.debug( "received quit event" );
						abortCommands();
						// Note: exception handling used for control
						throw new ParserQuitException();
					default:
						throw new RippleException(
							"event not yet supported: " + event );
				}
			}
		};

		RecognizerAdapter rc = new RecognizerAdapter(
			querySink, continuingQuerySink, commandSink, eventSink, qe.getErrorPrintStream() );

		Sink<Exception> parserExceptionSink = new Sink<Exception>()
		{
			public void put( final Exception e ) throws RippleException
			{
				// This happens, for instance, when the parser receives a value
				// which is too large for the target data type.  Non-fatal.
				if ( e instanceof NumberFormatException )
				{
					alert( e.toString() );
				}

				// Non-fatal.
				else if ( e instanceof antlr.RecognitionException )
				{
					alert( "RecognitionException: " + e.toString() );
				}

				// Non-fatal.
				else if ( e instanceof antlr.TokenStreamException )
				{
					alert( "TokenStreamException: " + e.toString() );
				}

				else
				{
					alert( "Error: " + e.toString() );
					throw new RippleException(
						"non-recoverable exception thrown: " + e.toString() );
				}
			}
		};

		// Pass input through a filter to watch for special byte sequences, and
		// another draw input through it even when the interface is busy.
		InputStream filter = new InputStreamEventFilter( is, rc );
		consoleReaderInput = new ThreadedInputStream( filter );

		String jlineDebugOutput = Ripple.jlineDebugOutput();

		// Create reader.
		try
		{
			reader = new ConsoleReader( consoleReaderInput,
				new OutputStreamWriter( qe.getPrintStream() ) );

			// Set up JLine logging if asked for.
			if ( null != jlineDebugOutput )
			{
				reader.setDebug(
					new PrintWriter(
						new FileWriter( jlineDebugOutput, true ) ) );
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		try
		{
			writeIn = new PipedInputStream();
			readOut = new PipedOutputStream( writeIn );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		// Initialize completors.
		updateCompletors();

		// Create interpreter.
		interpreter = new Interpreter( rc, writeIn, parserExceptionSink );
	}

	public void run() throws RippleException
	{
		lineNumber = 0;
		interpreter.parse();
//System.out.println( "done parsing" );
	}

	////////////////////////////////////////////////////////////////////////////

	private void readLine()
	{
		try
		{
			++lineNumber;
			String prefix = "" + lineNumber + " >>  ";
//System.out.println( "reading a line" );
//System.out.println( "    consoleReaderInput.available() = " + consoleReaderInput.available() );
			String line = reader.readLine( prefix );
//System.out.println( "done reading the line: " + line );
	
			if ( null != line )
			{
				// Feed the line to the lexer.
				byte[] bytes = line.getBytes();
				readOut.write( bytes, 0, bytes.length );
	
				// Add a newline character so the lexer will call readLine()
				// again when it gets there.
				readOut.write( EOL );
	
				readOut.flush();
			}
		}

		catch ( java.io.IOException e )
		{
			alert( "IOException: " + e.toString() );
		}
	}

	private void alert( final String s )
	{
		queryEngine.getErrorPrintStream().println( "\n" + s + "\n" );
	}

	private void updateCompletors()
	{
		LOGGER.debug( "updating completors" );
		List completors = new ArrayList();

		try
		{
			Lexicon lex = queryEngine.getLexicon();

			synchronized ( lex )
			{
				completors.add( lex.getCompletor() );
			}

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
				new DirectiveCompletor( directives ) );

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
			e.logError();
			LOGGER.error( "failed to update completors" );
		}
	}

	private class UpdateCompletorsCmd extends Command
	{
		public void execute( final QueryEngine qe, final ModelConnection mc )
			throws RippleException
		{
			updateCompletors();
		}

		protected void abort()
		{
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private void addCommand( final Command cmd )
	{
//System.out.println( "addCommand(" + cmd + ")" );
		cmd.setQueryEngine( queryEngine );
		taskQueue.add( cmd );
	}

	private void executeCommands() throws RippleException
	{
//System.out.println( "executeCommands()" );
		Scheduler.add( taskQueue );
	
		consoleReaderInput.setEager( true );
	
		try
		{
			taskQueue.waitUntilFinished();
		}
	
		catch ( RippleException e )
		{
			consoleReaderInput.setEager( false );
			throw e;
		}

		consoleReaderInput.setEager( false );
	}

	private void abortCommands()
	{
//System.out.println( "abortCommands()" );
		taskQueue.stop();
	}
}

// kate: tab-width 4
