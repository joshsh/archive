package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

import org.apache.log4j.Logger;

public abstract class ThreadWrapper
{
	private final static Logger s_logger
		= Logger.getLogger( ThreadWrapper.class );

	protected abstract void run() throws RippleException;

	private boolean finished;
	private RippleException error;

public boolean getFinished()
{
//s_logger.info( "--------- " + this + ".getFinished(): " + finished );
	return finished;
}

	public void setFinished( final boolean f )
	{
//s_logger.info( "--------- " + this + ".setFinished(" + f + ")" );
		finished = f;
	}

	public void start( final long timeout )
		throws RippleException
	{
		setFinished( false );
		error = null;

		final Object monitorObj = "";
		final ThreadWrapper tw = this;
//s_logger.info( "tw = " + tw );
//s_logger.info( "this = " + this );

		Thread t = new Thread( new Runnable()
		{
			public void run()
			{
//s_logger.info( "tw = " + tw );
				try
				{
s_logger.info( "--------- 1" );
s_logger.info( "tw = " + tw );
					tw.run();
s_logger.info( "tw = " + tw );
s_logger.info( "--------- 2" );
				}

				catch ( RippleException e )
				{
//s_logger.info( "--------- caught RippleException: " + e );
					error = e;
				}
//s_logger.info( "--------- 3" );

//s_logger.info( "tw = " + tw );
//synchronized( tw )
//{
				tw.setFinished( true );
//}
				synchronized( monitorObj )
				{
//s_logger.info( "--------- finished = " + tw.getFinished() );
					monitorObj.notify();
				}
			}
		} );

		t.start();

		try
		{
			synchronized( monitorObj )
			{
				// Wait up to the timeout if there is one.
				if ( timeout > 0 )
					monitorObj.wait( timeout );

				// If there is no timeout, wait indefinitely.
				else
					monitorObj.wait();
			}
//s_logger.info( "--------- 4 (finished = " + getFinished() + ")" );

			if ( !getFinished() )
				t.interrupt();
//s_logger.info( "--------- 5" );
		}

		catch ( InterruptedException e )
		{
//s_logger.info( "--------- 6" );
			throw new RippleException( e );
		}
//s_logger.info( "--------- 7 (finished = " + getFinished() + ")" );

		if ( null != error )
			throw error;

		else if ( !getFinished() )
			throw new RippleException( "operation expired (timeout = " + timeout + ")" );
//s_logger.info( "--------- 8" );
	}

	public void start()
		throws RippleException
	{
		start( -1 );
	}
}

// kate: tab-width 4
