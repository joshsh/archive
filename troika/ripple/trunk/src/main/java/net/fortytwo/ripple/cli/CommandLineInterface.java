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
import net.fortytwo.ripple.control.ThreadPool;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.Scheduler;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.CollectorHistory;
import net.fortytwo.ripple.util.Sink;
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
	final static Logger s_logger
		= Logger.getLogger( CommandLineInterface.class );

	PipedInputStream  writeIn;
	PipedOutputStream readOut;

	Interpreter interpreter;

	ConsoleReader reader;
	int lineNumber = 0;

	QueryEngine queryEngine;
	Scheduler scheduler;

	CollectorHistory<RippleList> queryResultHistory
		= new CollectorHistory<RippleList>( 2 );
boolean lastQueryContinued = false;

	////////////////////////////////////////////////////////////////////////////

	public CommandLineInterface( final QueryEngine qe, final InputStream is )
		throws RippleException
	{
		queryEngine = qe;
		scheduler = new Scheduler( queryEngine );

		// Handling of queries
		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
				Command cmd = createQueryCommand( ast, false );
				scheduler.add( cmd );
				scheduler.add( new UpdateCompletorsCmd() );
				waitUntilCommandsCompleted();
			}
		};

		// Handling of "continuing" queries
		Sink<ListAst> continuingQuerySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast )
				throws RippleException
			{
				Command cmd = createQueryCommand( ast, true );
				scheduler.add( cmd );
				scheduler.add( new UpdateCompletorsCmd() );
				waitUntilCommandsCompleted();
			}
		};

		// Handling of commands
		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd )
				throws RippleException
			{
				scheduler.add( cmd );
				scheduler.add( new UpdateCompletorsCmd() );
				waitUntilCommandsCompleted();
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
System.out.println( "Escape!" );
						scheduler.cancelCurrent();
						break;
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
{
e.printStackTrace( System.err );
					throw new RippleException(
						"non-recoverable exception thrown: " + e.toString() );
}
			}
		};

		// Pass input through a filter to watch for special byte sequences.
		InputStreamEventFilter filter = new InputStreamEventFilter( is, itf );

		String jlineDebugOutput = Ripple.jlineDebugOutput();

		// Create reader.
		try
		{
			reader = new ConsoleReader( filter,
				new OutputStreamWriter( qe.getPrintStream() ) );

			// Set up JLine logging if asked for.
			if ( null != jlineDebugOutput )
				reader.setDebug(
					new PrintWriter(
						new FileWriter( jlineDebugOutput, true ) ) );

			writeIn = new PipedInputStream();
			readOut = new PipedOutputStream( writeIn );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		// Initialize completors.
		updateCompletors();

		// Create interpreter.
		interpreter = new Interpreter( itf, writeIn, parserExceptionSink );
	}

	public void run()
		throws RippleException
	{
		interpreter.parse();
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
				ContainerTreeView view = new ContainerTreeView(
					queryEngine.getPrintStream(), mc );
				Sink<RippleList> med = doBuffer
					? new Buffer<RippleList>( view )
					: view;

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
				cmd.execute( queryEngine, mc );
				synchronized( cmd )
				{
					try
					{
						cmd.wait();
					}

					catch ( java.lang.InterruptedException e )
					{
						throw new RippleException( "interrupted while waiting for inner command to complete" );
					}
				}

				// Flush results to the view.
				if ( doBuffer )
					( (Buffer<RippleList>) results ).flush();
	
				if ( view.size() > 0 )
					queryEngine.getPrintStream().println( "" );
	
				queryResultHistory.advance();

				finished();
			}

			protected void abort()
			{
				if ( null != cmd )
					cmd.cancel();
			}
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
			s_logger.error( "Failed to update completors.  Continuing nonetheless." );
		}
	}

	void alert( String s )
	{
		queryEngine.getErrorPrintStream().println( "\n" + s + "\n" );
	}

	void waitUntilCommandsCompleted()
		throws RippleException
	{
		synchronized( scheduler )
		{
			try
			{
				while ( scheduler.count() > 0 )
					scheduler.wait();
			}

			catch ( InterruptedException e )
			{
				throw new RippleException( "interrupted while waiting for command queue to empty" );
			}
		}
	}

	private class UpdateCompletorsCmd extends Command
	{
		public void execute( final QueryEngine qe, final ModelConnection mc )
			throws RippleException
		{
			updateCompletors();

			finished();
		}

		protected void abort() {}
	}

	private class InputStreamEventFilter extends InputStream
	{
		InputStream source;
		RecognizerInterface itf;

		int buffered;

		public InputStreamEventFilter( final InputStream is,
										final RecognizerInterface itf )
		{
			source = is;
			this.itf = itf;

			buffered = -1;
		}

		public int read()
			throws java.io.IOException
		{
			if ( -1 != buffered )
			{
				int tmp = buffered;
				buffered = -1;
				return tmp;
			}

			// Break out when an ordinary character or sequence is found
			while( true )
			{
				int c = source.read();
	
				if ( 27 == c )
				{
					c = source.read();
	
					if ( 27 == c )
						itf.putEvent( RecognizerEvent.ESCAPE );
					else
					{
						buffered = c;
						return 27;
					}
				}
	
				else
					return c;
			}
		}
	}
}

// kate: tab-width 4
