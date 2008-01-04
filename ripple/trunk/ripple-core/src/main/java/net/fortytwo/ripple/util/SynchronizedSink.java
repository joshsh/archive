/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class SynchronizedSink<T> implements Sink<T>
{
	private Object synch;
	private Sink<T> sink;

	public SynchronizedSink( final Sink<T> other, final Object synch )
	{
		sink = other;
		this.synch = synch;
	}
	
	public SynchronizedSink( final Sink<T> other )
	{
		this( other, other );
	}

	public void put( final T t ) throws RippleException
	{
		synchronized ( synch )
		{
			sink.put( t );
		}
	}
}

// kate: tab-width 4
