package net.fortytwo.ripple.query;

import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.control.Task;
import net.fortytwo.ripple.control.ThreadPool;

public class Scheduler
{
	QueryEngine queryEngine;
	ExecutorRunnable schedulerRunnable;
	LinkedList<Command> queue;

	public Scheduler( final QueryEngine qe )
	{
		queryEngine = qe;
		queue = new LinkedList<Command>();

		schedulerRunnable = new ExecutorRunnable();
		Thread t = ThreadPool.getThread( schedulerRunnable, "scheduler thread" );
		t.start();
	}

	public void add( final Command cmd )
	{
//System.out.println( "add( " + cmd + " )" );
		synchronized( queue )
		{
			queue.addLast( cmd );
		}

		synchronized( schedulerRunnable )
		{
			schedulerRunnable.notify();
		}
	}

	/**
	 *  Aborts the currently executing command (provided that it supports early exit).
     */
	public void cancelCurrent()
	{
		schedulerRunnable.cancelCurrent();
	}

	public void cancelAll()
	{
		synchronized( queue )
		{
			queue.clear();
		}

		schedulerRunnable.cancelCurrent();
	}

	private class ExecutorRunnable implements Runnable
	{
		// Currently executing command.
		Command cmd = null;

		public void run()
		{
			// Never terminate unless interrupted
			while( true )
			{
//System.out.println( "--- 1" );
				int size;

				synchronized( queue )
				{
					size = queue.size();
				}

				// Wait while there are no items in the queue.
				if ( 0 == size )
				{
					synchronized( this )
					{
						try
						{
//System.out.println( "waiting for new commands" );
							wait();
//System.out.println( "done waiting" );
						}

						catch( InterruptedException e )
						{
							System.err.println( "scheduler interrupted while waiting for new commands" );
							return;
						}
					}
				}
//System.out.println( "--- 2" );

				synchronized( queue )
				{
//System.out.println( "cmd = queue.removeFirst();" );
					cmd = queue.removeFirst();
				}
//System.out.println( "--- 3" );

				// Begin executing the command.
				queryEngine.executeCommand( cmd );
//System.out.println( "--- 4" );

				//  Wait until the command has finished executing.
				try
				{
					synchronized( cmd )
					{
						if ( !cmd.isFinished() )
							cmd.wait();
					}
				}

				catch( InterruptedException e )
				{
					System.err.println( "scheduler interrupted while waiting for command to finish executing" );
					return;
				}
//System.out.println( "--- 5" );

				synchronized( this )
				{
					cmd = null;
				}
//System.out.println( "--- 6" );
			}
		}

		synchronized public void cancelCurrent()
		{
//System.out.println( "cancelCurrent() -- cmd = " + cmd );
			if ( null != cmd )
				cmd.cancel();
		}
	}














	public static void schedule( final Task task, final Sink<Task> completedTaskSink )
	{
		Runnable target = new Runnable()
		{
			public void run()
			{
				try
				{
					task.execute();

					// Now that execute() is done, the task is done.
//System.out.println( "task is done." );
					completedTaskSink.put( task );
				}

				catch ( RippleException e )
				{
System.err.println( "RippleException ignored: " + e );
				}
			}
		};

		// For now, we create and start a thread immediately.
		Thread t = ThreadPool.getThread( target, "for Scheduler schedule: " + task );
		t.start();
	}
}

// kate: tab-width 4
