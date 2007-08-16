/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.control;

import net.fortytwo.ripple.RippleException;
//import net.fortytwo.ripple.query.Scheduler;
import net.fortytwo.ripple.util.Sink;

public class TaskSet
{
	int count = 0;

	public void add( final Task task )
	{
		synchronized( completedTaskSink )
		{
			count++;
System.out.println( "[+] count is now: " + count );
		}

		Scheduler.add( task, completedTaskSink );
	}

	/**
	 *  Note: by the time this method is called, all tasks should already
	 *        have been added to the set, while there may be any number of
	 *        tasks which have not finished executing.
	 */
	public void waitUntilEmpty() throws RippleException
	{
System.out.println( "waitUntilEmpty" );
// TODO: slight chance of a race condition here
		while ( count > 0 )
		{
			try
			{
				synchronized( completedTaskSink )
				{
					completedTaskSink.wait();
				}
			}

			catch ( java.lang.InterruptedException e )
			{
				throw new RippleException( "interrupted while waiting to complete tasks" );
			}
		}
	}

	Sink<Task> completedTaskSink = new Sink<Task>()
	{
		public synchronized void put( final Task task ) throws RippleException
		{
			count--;
System.out.println( "[-] count is now: " + count );

			notify();
		}
	};
}

// kate: tab-width 4
