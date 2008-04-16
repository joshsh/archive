/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.control;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

import org.apache.log4j.Logger;

public abstract class Task
{
	private static final Logger LOGGER = Logger.getLogger( Task.class );

	private LinkedList<Task> children = null;
	private boolean finished = true, stopped = false;

	protected abstract void executeProtected() throws RippleException;
	protected abstract void stopProtected();

	public void execute() throws RippleException
	{
//System.out.println( "[" + this + "].execute()" );
		// A task may be stopped before it executes.
		if ( !stopped )
		{
			executeProtected();
		}
//else
//System.out.println( "    already stopped!" );

		synchronized ( this )
		{
			finished = true;

			notify();
		}
	}

	/**
	 * Note: it is possible to stop a task which has already finished executing
	 * (the effect is to stop any children which may still be executing).
	 */
	public synchronized void stop()
	{
//System.out.println( "[" + this + "].stop()" );
		if ( !stopped )
		{
			stopped = true;
	
			stopProtected();
	
			if ( null != children )
			{
				Iterator<Task> iter = children.iterator();
				while ( iter.hasNext() )
				{
					Task child = iter.next();
//System.out.println( "    stopping child: " + child );
					child.stop();
				}
			}
		}
	}

	/**
	 * Note: should not be called outside of Scheduler.
	 */
	public void begin()
	{
		finished = false;
		stopped = false;

		// Note: while executing a task a second time is permitted, the task loses
		// ownership of any children acquired through a previous execution.
		if ( null != children )
		{
			children.clear();
		}
	}

	/**
	 * Note: should not be called outside of Scheduler.
	 */
	public synchronized void addChild( final Task child )
	{
//System.out.println( "[" + this + "].addChild(" + child + ")" );
		if ( finished )
		{
			LOGGER.error( "attempted to add a child to a finished task" );
		}

		else
		{
			if ( null == children )
			{
				children = new LinkedList<Task>();
			}
			
			children.add( child );
		}
	}

	public void waitUntilFinished() throws RippleException
	{
//System.out.println( "[" + this + "].waitUntilFinished()" );
		synchronized ( this )
		{
			if ( !finished )
			{
				try
				{
					wait();
				}
	
				catch ( InterruptedException e )
				{
					throw new RippleException( "interrupted while waiting for task to finish" );
				}
			}
		}
//System.out.println( "    done -- " + this );
	}
}

// kate: tab-width 4
