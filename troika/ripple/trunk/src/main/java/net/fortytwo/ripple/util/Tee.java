package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class Tee<T> implements Sink<T>
{
	Sink<T> sinkA, sinkB;

	public Tee( final Sink<T> sinkA, final Sink<T> sinkB )
	{
		this.sinkA = sinkA;
		this.sinkB = sinkB;
	}

	public void put( T t )
		throws RippleException
	{
		sinkA.put( t );
		sinkB.put( t );
	}
}

// kate: tab-width 4
