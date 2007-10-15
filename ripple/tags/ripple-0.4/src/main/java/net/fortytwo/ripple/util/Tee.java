/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class Tee<T> implements Sink<T>
{
	private Sink<T> sinkA, sinkB;

	public Tee( final Sink<T> sinkA, final Sink<T> sinkB )
	{
		this.sinkA = sinkA;
		this.sinkB = sinkB;
	}

	public void put( final  T t )
		throws RippleException
	{
		sinkA.put( t );
		sinkB.put( t );
	}
}

// kate: tab-width 4
