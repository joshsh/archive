package net.fortytwo.ripple.query;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.cli.ParserExceptionSink;
import net.fortytwo.ripple.cli.RecognizerAdapter;
import net.fortytwo.ripple.cli.RecognizerEvent;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.CollectorHistory;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

public class QueryPipe implements Sink<String>
{
	private static final long WAIT_INTERVAL = 100l;
	
	private Interpreter interpreter;

	private PipedIOStream writeIn = new PipedIOStream();
//	private PipedInputStream  writeIn;
//	private PipedOutputStream readOut;

	private Buffer<RippleList> resultBuffer;
	
	private CollectorHistory<RippleList> queryResultHistory
		= new CollectorHistory<RippleList>( 2 );
	private boolean lastQueryContinued = false;

	private Thread interpreterThread;
	
	public QueryPipe( final QueryEngine queryEngine, final Sink<RippleList> resultSink ) throws RippleException
	{
/*final Sink<RippleList> tempSink = new Sink<RippleList>() {
	public void put( final RippleList l ) throws RippleException {
		System.out.println( "received list: " + l );
		resultBuffer.put(l);
	}
};*/
		resultBuffer = new Buffer<RippleList>( resultSink );
		
		// Handling of queries
		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast ) throws RippleException
			{
System.out.println( "### received: " + ast );
				Collector<RippleList> nilSource = new Collector<RippleList>();
				nilSource.put( RippleList.NIL );
				Source<RippleList> composedWith = lastQueryContinued
						? queryResultHistory.getSource( 1 ) : nilSource;
			
				ModelConnection mc = queryEngine.getConnection();
				new RippleQueryCmd( ast, resultBuffer, composedWith ).execute( queryEngine, mc );
				mc.close();
				
				lastQueryContinued = false;
			}
		};

		// Handling of "continuing" queries
		Sink<ListAst> continuingQuerySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast ) throws RippleException
			{
				Collector<RippleList> nilSource = new Collector<RippleList>();
				nilSource.put( RippleList.NIL );
				Source<RippleList> composedWith = lastQueryContinued
						? queryResultHistory.getSource( 1 ) : nilSource;
	
				ModelConnection mc = queryEngine.getConnection();
				new RippleQueryCmd( ast, resultBuffer, composedWith ).execute( queryEngine, mc );
				mc.close();
				
				lastQueryContinued = true;
			}
		};

		// Handling of commands
		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd ) throws RippleException
			{
				ModelConnection mc = queryEngine.getConnection();
				cmd.execute( queryEngine, mc );
				mc.close();
			}
		};

		// Handling of parser events
		Sink<RecognizerEvent> eventSink = new Sink<RecognizerEvent>()
		{
			public void put( final RecognizerEvent event )
				throws RippleException
			{
				// (ignore events)
			}
		};
		
		RecognizerAdapter rc = new RecognizerAdapter(
				querySink, continuingQuerySink, commandSink, eventSink, queryEngine.getErrorPrintStream() );

		Sink<Exception> parserExceptionSink = new ParserExceptionSink(
				queryEngine.getErrorPrintStream() );
		
/*		writeIn = new PipedInputStream();
		try {
			readOut = new PipedOutputStream( writeIn );
		} catch ( IOException e ) {
			throw new RippleException( e );
		}*/
		
		// Create interpreter.
		interpreter = new Interpreter( rc, writeIn, parserExceptionSink );
		
		Runnable r = new Runnable() {
			public void run() {
				try
				{
					interpreter.parse();
				}
				
				catch ( RippleException e )
				{
					e.logError();
				}
			}
		};
		
		// Note: this thread is never terminated unless there is an error.
		interpreterThread = new Thread( r );
		interpreterThread.start();
	}
	
	public void close() throws RippleException
	{
		try
		{
//			readOut.close();
			writeIn.close();
		}
		
		catch ( IOException e )
		{
			throw new RippleException( e );
		}
	}
	
	public void put( final String expr ) throws RippleException
	{
		try
		{
			writeIn.write( expr.getBytes() );
//			readOut.write( expr.getBytes() );
//			readOut.flush();
		}
		
		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
		
		// Wait until the interpreter thread is idle
		do
		{
//System.out.println( "waiting " + WAIT_INTERVAL + " milliseconds" );
			synchronized ( this )
			{
				// FIXME: the first wait depends on a race condition
				try
				{
					wait( WAIT_INTERVAL );
				}
				
				catch ( InterruptedException e )
				{
					throw new RippleException( e );
				}
			}
		} while ( Thread.State.RUNNABLE == interpreterThread.getState() );
		
		resultBuffer.flush();
	}
}
