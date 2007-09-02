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
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.cli.jline.DirectiveCompletor;
import net.fortytwo.ripple.control.Scheduler;
import net.fortytwo.ripple.control.Task;
import net.fortytwo.ripple.control.TaskQueue;
import net.fortytwo.ripple.control.TaskSet;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.io.TurtleView;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.CollectorHistory;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.SynchronizedSink;
import net.fortytwo.ripple.util.Tee;

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
public class CommandLineInterface
{
	final static Logger logger
		= Logger.getLogger( CommandLineInterface.class );

	final static byte[] EOL = { '\n' };

	PipedInputStream  writeIn;
	PipedOutputStream readOut;
	ThreadedInputStream consoleReaderInput;

	Interpreter interpreter;

	ConsoleReader reader;
	int lineNumber;

	QueryEngine queryEngine;

	CollectorHistory<RippleList> queryResultHistory
		= new CollectorHistory<RippleList>( 2 );
boolean lastQueryContinued = false;

	TaskQueue taskQueue = new TaskQueue();

	////////////////////////////////////////////////////////////////////////////

	public CommandLineInterface( final QueryEngine qe, final InputStream is )
		throws RippleException
	{
		queryEngine = qe;

		// Handling of queries
		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
				addCommand( createQueryCommand( ast, false ) );
				addCommand( new UpdateCompletorsCmd() );
				executeCommands();
			}
		};

		// Handling of "continuing" queries
		Sink<ListAst> continuingQuerySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
				addCommand( createQueryCommand( ast, true ) );
				addCommand( new UpdateCompletorsCmd() );
				executeCommands();
			}
		};

		// Handling of commands
		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd )
				throws RippleException
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
						logger.debug( "received escape event" );
						abortCommands();
						break;
					case QUIT:
						logger.debug( "received quit event" );
						abortCommands();
						// Note: exception handling used for control
						throw new ParserQuitException();
					default:
						throw new RippleException(
							"event not yet supported: " + event );
				}
			}
		};

		RecognizerInterface itf = new RecognizerInterface(
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
		InputStream filter = new InputStreamEventFilter( is, itf );
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
		interpreter = new Interpreter( itf, writeIn, parserExceptionSink );
	}

	public void run() throws RippleException
	{
		lineNumber = 0;
		interpreter.parse();
//System.out.println( "done parsing" );
	}

	////////////////////////////////////////////////////////////////////////////

	void readLine()
	{
		try
		{
			++lineNumber;
			String prefix = "" + lineNumber + " >>  ";
			String line = reader.readLine( prefix );
	
			if ( null != line )
			{
				// Feed the line to the lexer.
				byte[] bytes = line.getBytes();
				readOut.write( bytes, 0, bytes.length );
	
				// Add a newline character so the lexer will call readLine()
				// again when it gets there.
				readOut.write( EOL, 0, 1 );
	
				readOut.flush();
			}
		}

		catch ( java.io.IOException e )
		{
			alert( "IOException: " + e.toString() );
		}
	}

	void alert( String s )
	{
		queryEngine.getErrorPrintStream().println( "\n" + s + "\n" );
	}

	void updateCompletors()
	{
		logger.debug( "updating completors" );
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
			logger.error( "failed to update completors" );
		}
	}

	private class UpdateCompletorsCmd extends Command
	{
		public void execute( final QueryEngine qe, final ModelConnection mc )
			throws RippleException
		{
			updateCompletors();
		}

		protected void abort() {}
	}

	////////////////////////////////////////////////////////////////////////////

	void addCommand( final Command cmd )
	{
		cmd.setQueryEngine( queryEngine );
		taskQueue.add( cmd );
	}

	void executeCommands() throws RippleException
	{
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

	void abortCommands()
	{
		taskQueue.stop();
	}

	Command createQueryCommand( final ListAst ast, final boolean continuing )
	{
		return new Command()
		{
			Command cmd = null;

			public void execute( QueryEngine qe, ModelConnection mc )
				throws RippleException
			{
				boolean doBuffer = Ripple.containerViewBufferOutput();
	
				queryEngine.getPrintStream().println( "" );
	
				// Results are first dereferenced, then placed into a buffer which
				// will be flushed into the view after the lexicon is updated.
				TurtleView view = new TurtleView(
					queryEngine.getPrintStream(), mc );
				Sink<RippleList> med = new SynchronizedSink(
					( doBuffer
						? new Buffer<RippleList>( view )
						: view ) );

				final Sink<RippleList> results
					= new Tee<RippleList>( med, queryResultHistory );
	
				final ModelConnection mcf = mc;
				Sink<RippleList> derefSink = new Sink<RippleList>()
				{
					public void put( final RippleList list) throws RippleException
					{
						dereference( list.getFirst(), mcf );
						results.put( list );
					}
				};

Collector<RippleList> nilSource = new Collector<RippleList>();
nilSource.put( RippleList.NIL );
				cmd = new RippleQueryCmd( ast, derefSink,
					( lastQueryContinued
						? queryResultHistory.getSource( 1 )
						: nilSource ) );
lastQueryContinued = continuing;

				// Execute the inner command and wait until it is finished.
				cmd.setQueryEngine( queryEngine );
				TaskSet ts = new TaskSet();
				ts.add( cmd );
				ts.waitUntilEmpty();

				// Flush results to the view.
				if ( doBuffer )
					( (Buffer<RippleList>) results ).flush();
	
				if ( view.size() > 0 )
					queryEngine.getPrintStream().println( "" );
	
				queryResultHistory.advance();
			}

			protected void abort() {}
		};
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
			// (soft fail... don't even log the error)
		}
	}
}

// kate: tab-width 4
