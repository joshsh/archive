/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.flow.Sink;

public class Tee<T, E extends Exception> implements Sink<T, E>
{
	private Sink<T, E> sinkA, sinkB;

	public Tee( final Sink<T, E> sinkA, final Sink<T, E> sinkB )
	{
		this.sinkA = sinkA;
		this.sinkB = sinkB;
	}

	public void put( final  T t ) throws E
	{
		sinkA.put( t );
		sinkB.put( t );
	}
}
