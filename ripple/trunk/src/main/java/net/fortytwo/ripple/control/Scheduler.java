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
import net.fortytwo.ripple.util.NullSink;
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

	public static void add( final Task task )
	{
		add( task, new NullSink<Task>() );
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
		task.begin();

//System.out.println( "[" + this + "]add( " + task + ", ... )" );
		// Add the new task as a child of the currently executing task.
		Thread currentThread = Thread.currentThread();
		if ( currentThread instanceof WorkerThread )
		{
			Task parent = ( (WorkerThread) currentThread ).getCurrentTask();
//System.out.println( "    parent = " + parent );
			parent.addChild( task );
		}

		TaskItem taskItem = new TaskItem( task, completedTaskSink );

		synchronized ( taskQueue )
		{
			taskQueue.addLast( taskItem );

			// When the queue goes from empty to nonempty, there may be a number
			// of threads waiting for a task.  Notify the first in line that
			// a task is available.
			if ( 1 == taskQueue.size() && waitingRunnables.size() > 0 )
			{
//System.out.println( "    ( 1 == taskQueue.size() && waitingRunnables.size() > 0 )" );
				WorkerRunnable r = waitingRunnables.removeFirst();
				synchronized ( r )
				{
					r.notify();
				}
			}

			// If there are more tasks than threads, and we have not reached the
			// maximum number of threads, then create a new one.
			else if ( allRunnables.size() < maxThreads )
			{
//System.out.println( "    taskQueue.size() > allRunnables.size() && allRunnables.size() < maxThreads" );
				WorkerRunnable r = new WorkerRunnable();
				allRunnables.add( r );
				Thread t = new WorkerThread( r );
				t.start();
			}
//else
//System.out.println( "Could not start a new thread!" );
		}
//System.out.println( "    ### total number of worker runnables: " + allRunnables.size() );
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

	private class WorkerThread extends Thread
	{
		WorkerRunnable runnable;

		public WorkerThread( final WorkerRunnable r )
		{
			super( r );

			runnable = r;
		}

		public Task getCurrentTask()
		{
			return runnable.getCurrentTask();
		}
	}

	private class WorkerRunnable implements Runnable
	{
		Task currentTask = null;

		public void run()
		{
//System.out.println( "[" + this + "]run()" );
			// Continue waiting for and executing tasks indefinitely.
			while ( true )
			{
				TaskItem taskItem = null;

				synchronized ( taskQueue )
				{
//System.out.println( "    testing queue" );
					if ( taskQueue.size() > 0 )
						taskItem = taskQueue.removeFirst();
				}

				// If a task was found in the queue, execute it.
				if ( null != taskItem )
				{
//System.out.println( "    found a task to execute" );
					try
					{
						currentTask = taskItem.task;

//System.out.println( "    executing task: " + currentTask );
						currentTask.execute();
						taskItem.sink.put( taskItem.task );
					}
		
					catch ( RippleException e )
					{
//System.err.println( "Error executing task: " + e );
						e.logError();
					}
					
					catch ( Throwable t )
					{
						if ( t instanceof InterruptedException )
							logger.warn( "task interrupted: " + currentTask );
// 						else
// 							...
					}
				}

				// If there are no tasks in the queue, add this Runnable to a
				// list and wait.
				else
				{
//System.out.println( "    adding self to waiting queue" );
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
							logger.warn( "worker runnable interrupted while waiting for new tasks" );
						}
					}
				}
			}
		}

		public Task getCurrentTask()
		{
			return currentTask;
		}
	}
}

// kate: tab-width 4
