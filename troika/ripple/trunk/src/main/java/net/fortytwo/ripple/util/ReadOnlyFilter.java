package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public abstract class ReadOnlyFilter<T> implements Sink<T>
{
	Sink<T> sink;

	public ReadOnlyFilter( final Sink<T> sink )
	{
		this.sink = sink;
	}

	public void put( T t )
		throws RippleException
	{
		handle( t );
		sink.put( t );
	}

	public abstract void handle( T t )
		throws RippleException;
}

// kate: tab-width 4
