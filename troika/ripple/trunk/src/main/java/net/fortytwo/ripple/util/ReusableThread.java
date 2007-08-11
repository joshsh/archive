package net.fortytwo.ripple.util;

public class ReusableThread extends Thread
{
	Runnable target;

	public ReusableThread( final Runnable target, final String name )
	{
		super( name );
		this.target = target;
	}

//		public void setRunnable( final Runnable target ) ...

	public void run()
	{
		try
		{
			target.run();
		}

		// Avoid thread leakage.
		catch ( RuntimeException e )
		{
System.err.println( "RuntimeException ignored: " + e );
		}
	}
}

// kate: tab-width 4
