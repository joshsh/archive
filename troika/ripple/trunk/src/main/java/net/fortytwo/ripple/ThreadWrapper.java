package net.fortytwo.ripple;

public abstract class ThreadWrapper
{
	protected abstract void run() throws RippleException;

	private boolean finished;
	private RippleException error;

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
{
System.out.println( "wait(" + timeout + ")" );
					monitorObj.wait( timeout );
}
				else
{
					monitorObj.wait();
System.out.println( "wait()" );
}
System.out.println( "done! #####################" ); System.out.flush();
			}

			if ( !finished )
				t.interrupt();
		}

		catch ( InterruptedException e )
		{
System.out.println( "catch ( InterruptedException e )" ); System.out.flush();
			throw new RippleException( e );
		}
System.out.println( "finished = " + finished );

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
