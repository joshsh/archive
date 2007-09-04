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
	Sink<T> sink;

	public SynchronizedSink( final Sink<T> other )
	{
		sink = other;
	}

	public void put( final T t ) throws RippleException
	{
		synchronized ( sink )
		{
			sink.put( t );
		}
	}
}

// kate: tab-width 4
