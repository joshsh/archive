package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public abstract class ThreadWrapper
{
	protected abstract void run() throws RippleException;

	boolean finished;
	RippleException error;

	public void start( final long timeout )
		throws RippleException
	{
		finished = false;
		error = null;

		final Object monitorObj = "";
		final ThreadWrapper tw = this;

		Thread t = new Thread( new Runnable() {
			public void run()
			{
				try
				{
					tw.run();
				}

				catch ( RippleException e )
				{
					error = e;
				}

				finished = true;
				synchronized( monitorObj )
				{
					monitorObj.notify();
				}
			}
		} );

		t.start();

		try
		{
			synchronized( monitorObj )
			{
				if ( timeout > 0 )
					monitorObj.wait( timeout );
				else
					monitorObj.wait();
			}

			if ( !finished )
				t.interrupt();
		}

		catch ( InterruptedException e )
		{
			throw new RippleException( e );
		}

		if ( !finished )
			throw new RippleException( "operation timed out" );

		else if ( null != error )
			throw error;
	}

	public void start()
		throws RippleException
	{
		start( -1 );
	}
}

// kate: tab-width 4
