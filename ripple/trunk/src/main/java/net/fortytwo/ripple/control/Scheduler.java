/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.control;

import java.util.LinkedList;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

public class Scheduler
{
	final static Logger logger = Logger.getLogger( Scheduler.class );

	static Scheduler singleInstance = null;

	LinkedList<TaskItem> taskQueue;
	LinkedList<WorkerRunnable> allRunnables;
	LinkedList<WorkerRunnable> waitingRunnables;
	int maxThreads;

	////////////////////////////////////////////////////////////////////////////

	public static void add( final Task task, final Sink<Task> completedTaskSink )
	{
		if ( null == singleInstance )
		{
			singleInstance = new Scheduler();
		}

		singleInstance.addPrivate( task, completedTaskSink );
	}

	////////////////////////////////////////////////////////////////////////////

	Scheduler()
	{
		taskQueue = new LinkedList<TaskItem>();
		allRunnables = new LinkedList<WorkerRunnable>();
		waitingRunnables = new LinkedList<WorkerRunnable>();

		maxThreads = Ripple.maxWorkerThreads();
	}

	void addPrivate( final Task task, final Sink<Task> completedTaskSink )
	{
		TaskItem taskItem = new TaskItem( task, completedTaskSink );

		synchronized ( taskQueue )
		{
			taskQueue.addLast( taskItem );

			// When the queue goes from empty to nonempty, there may be a number
			// of threads waiting for a task.  Notify the first in line that
			// a task is available.
			if ( 1 == taskQueue.size() && waitingRunnables.size() > 0 )
			{
				WorkerRunnable r = waitingRunnables.removeFirst();
				synchronized ( r )
				{
					r.notify();
				}
			}

			// If there are more tasks than threads, and we have not reached the
			// maximum number of threads, then create a new one.
			if ( taskQueue.size() > allRunnables.size()
					&& allRunnables.size() < maxThreads )
			{
				WorkerRunnable r = new WorkerRunnable();
				allRunnables.add( r );
				Thread t = new Thread( r );
				t.start();
			}
		}
System.out.println( "### total number of worker runnables: " + allRunnables.size() );
	}

	////////////////////////////////////////////////////////////////////////////

	private class TaskItem
	{
		public TaskItem( final Task task, final Sink<Task> sink )
		{
			this.task = task;
			this.sink = sink;
		}

		public Task task;
		public Sink<Task> sink;
	}

	private class WorkerRunnable implements Runnable
	{
		public void run()
		{
			// Continue waiting for and executing tasks indefinitely.
			while ( true )
			{
				TaskItem taskItem = null;

				synchronized ( taskQueue )
				{
					if ( taskQueue.size() > 0 )
						taskItem = taskQueue.removeFirst();
				}

				// If a task was found in the queue, execute it.
				if ( null != taskItem )
				{
					try
					{
						taskItem.task.execute();
						taskItem.sink.put( taskItem.task );
					}
		
					catch ( RippleException e )
					{
System.err.println( "Error executing task: " + e );
						e.logError();
					}

					catch ( InterruptedException e )
					{
						
// 					catch ( Throwable t )
// 					{
// 						...
// 					}
				}

				// If there are no tasks in the queue, add this Runnable to a
				// list and wait.
				else
				{
					synchronized ( waitingRunnables )
					{
						waitingRunnables.addLast( this );
					}

					synchronized ( this )
					{
						try
						{
							wait();
						}

						catch ( InterruptedException e )
						{
							System.err.println( "Warning: worker runnable interrupted while waiting for new tasks" );
						}
					}
				}
			}
		}
	}
}

// kate: tab-width 4
