package net.fortytwo.ripple.query;

import java.util.LinkedList;

public class CommandQueue
{
	QueryEngine queryEngine;
	ExecutorRunnable executorRunnable;
	Thread executorThread;
	LinkedList<Command> queue;

	public CommandQueue( final QueryEngine qe )
	{
		queryEngine = qe;
		queue = new LinkedList<Command>();
		executorRunnable = null;

		executorRunnable = new ExecutorRunnable();
		resetThread();
	}

	public void add( final Command cmd )
	{
		synchronized( queue )
		{
			queue.addLast( cmd );
		}

		synchronized( executorRunnable )
		{
			executorRunnable.notify();
		}
	}

	public void cancelCurrent()
	{
		resetThread();
	}

	public void cancelAll()
	{
		synchronized( queue )
		{
			queue.clear();
		}

		resetThread();
	}

	void resetThread()
	{
		if ( null != executorThread )
			executorThread.interrupt();

		executorThread = new Thread( executorRunnable );
		executorThread.start();
	}

	private class ExecutorRunnable implements Runnable
	{
		public void run()
		{
			// Never terminate unless interrupted
			while( true )
			{
				// Wait while there are no items in the queue
				if ( 0 == queue.size() )
				{
					synchronized( this )
					{
						try
						{
							wait();
						}

						catch( InterruptedException e )
						{
							System.err.println( "executor interrupted while waiting for new commands" );
							return;
						}
					}
				}

				Command cmd;

				synchronized( queue )
				{
					cmd = queue.removeFirst();
				}

				queryEngine.executeCommand( cmd );
			}
		}
	}
}

// kate: tab-width 4
