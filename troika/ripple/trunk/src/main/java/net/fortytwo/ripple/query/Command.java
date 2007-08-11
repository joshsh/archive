package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public abstract class Command
{
	public abstract void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException;

	protected abstract void abort();

	synchronized public void cancel()
	{
		abort();
		notify();
	}

	boolean f = false;

	synchronized protected void finished()
	{
		f = true;
		notify();
	}

	public boolean isFinished()
	{
		return f;
	}
}

// kate: tab-width 4
