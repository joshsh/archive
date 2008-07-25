/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.cli.ParserExceptionSink;
import net.fortytwo.ripple.cli.RecognizerAdapter;
import net.fortytwo.ripple.cli.RecognizerEvent;
import net.fortytwo.ripple.cli.ast.ListAST;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.flow.Buffer;
import net.fortytwo.ripple.flow.Collector;
import net.fortytwo.ripple.flow.CollectorHistory;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.flow.Source;

import java.io.IOException;

public class QueryPipeThreaded implements Sink<String, RippleException>
{
	private static final long WAIT_INTERVAL = 100l;

	private Interpreter interpreter;
	private RecognizerAdapter recognizerAdapter;

	private PipedIOStream inOut;

	private Buffer<RippleList, RippleException> resultBuffer;

	private CollectorHistory<RippleList, RippleException> queryResultHistory
		= new CollectorHistory<RippleList, RippleException>( 2 );
	private boolean lastQueryContinued = false;

	private Thread interpreterThread;

	public QueryPipeThreaded( final QueryEngine queryEngine, final Sink<RippleList, RippleException> resultSink ) throws RippleException
	{
		 inOut = new PipedIOStream();

		resultBuffer = new Buffer<RippleList, RippleException>( resultSink );
		final Object mutex = resultBuffer;

		final Collector<RippleList, RippleException> nilSource = new Collector<RippleList, RippleException>();
		// FIXME: this is stupid
		ModelConnection mc = queryEngine.getConnection();
		nilSource.put( mc.list() );
		mc.close();

		// Handling of queries
		Sink<ListAST, RippleException> querySink = new Sink<ListAST, RippleException>()
		{
			public void put( final ListAST ast ) throws RippleException
			{
//System.out.println( "### received: " + ast );
				synchronized ( mutex )
				{
					Source<RippleList, RippleException> composedWith = lastQueryContinued
							? queryResultHistory.getSource( 1 ) : nilSource;

					ModelConnection mc = queryEngine.getConnection();
                    try {
                        new RippleQueryCmd( ast, resultBuffer, composedWith ).execute( queryEngine, mc );
                    } finally {
                        mc.close();
                    }

                    lastQueryContinued = false;
					queryResultHistory.advance();
				}
			}
		};

		// Handling of "continuing" queries
		Sink<ListAST, RippleException> continuingQuerySink = new Sink<ListAST, RippleException>()
		{
			public void put( final ListAST ast ) throws RippleException
			{
				synchronized ( mutex )
				{
					Source<RippleList, RippleException> composedWith = lastQueryContinued
							? queryResultHistory.getSource( 1 ) : nilSource;

					ModelConnection mc = queryEngine.getConnection();
                    try {
                        new RippleQueryCmd( ast, resultBuffer, composedWith ).execute( queryEngine, mc );
                    } finally {
                        mc.close();
                    }

                    lastQueryContinued = true;
					queryResultHistory.advance();
				}
			}
		};

		// Handling of commands
		Sink<Command, RippleException> commandSink = new Sink<Command, RippleException>()
		{
			public void put( final Command cmd ) throws RippleException
			{
				ModelConnection mc = queryEngine.getConnection();
                try {
                    cmd.execute( queryEngine, mc );
                } finally {
                    mc.close();
                }
            }
		};

		// Handling of parser events
		Sink<RecognizerEvent, RippleException> eventSink = new Sink<RecognizerEvent, RippleException>()
		{
			public void put( final RecognizerEvent event )
				throws RippleException
			{
				/*if ( RecognizerEvent.QUIT == event )
				{
					throw new ParserQuitException();
				}*/

				// (ignore other events)
			}
		};

		recognizerAdapter = new RecognizerAdapter(
				querySink, continuingQuerySink, commandSink, eventSink, queryEngine.getErrorPrintStream() );

		Sink<Exception, RippleException> parserExceptionSink = new ParserExceptionSink(
				queryEngine.getErrorPrintStream() );

/*		inOut = new PipedInputStream();
		try {
			readOut = new PipedOutputStream( inOut );
		} catch ( IOException e ) {
			throw new RippleException( e );
		}*/

		// Create interpreter.
		interpreter = new Interpreter( recognizerAdapter, inOut, parserExceptionSink );
//System.out.println("main thread: " + Thread.currentThread());

		Runnable r = new Runnable() {
			public void run() {
				try
				{
//System.out.println("parser thread: " + Thread.currentThread());
					interpreter.parse();
				}

				catch ( RippleException e )
				{
					e.logError();
				}
			}
		};

		// Note: this thread is never terminated unless there is an error.
		interpreterThread = new Thread( r, "for QueryPipeThreaded" );
		interpreterThread.start();
	}

	public void close() throws RippleException
	{
//System.out.println("CLOSING THE PIPE");
		try
		{
			//recognizerAdapter.putEvent( RecognizerEvent.QUIT );
			interpreter.quit();
			interpreterThread.interrupt();
//System.out.println("CLOSING THE BUFFER");
			inOut.close();
		}

		catch ( IOException e )
		{
			throw new RippleException( e );
		}
	}

	public void put( final String expr ) throws RippleException
	{
//System.out.println("evaluating: " + expr);
		try
		{
			inOut.write( expr.getBytes() );
		}

		catch ( IOException e )
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
