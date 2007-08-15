package net.fortytwo.ripple.control;

import org.apache.log4j.Logger;

public class ThreadPool
{
	final static Logger logger
		= Logger.getLogger( ThreadPool.class );

	public static Thread getThread( final Runnable target, final String name )
	{
		Thread t = new ReusableThread( target, name );
logger.info( "creating new thread '" + name + "' -- " + t );
		return t;
	}
}

// kate: tab-width 4
