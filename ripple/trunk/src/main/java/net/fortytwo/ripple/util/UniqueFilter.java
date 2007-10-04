/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.util.HashSet;
import java.util.Set;

import net.fortytwo.ripple.RippleException;

public class UniqueFilter<T> implements Sink<T>
{
	private Set<T> set = new HashSet<T>();
	private Sink<T> sink;

	public UniqueFilter( final Sink sink )
	{
		this.sink = sink;
	}

	public void put( final T t ) throws RippleException
	{
		if ( set.add( t ) )
		{
			sink.put( t );
		}
	}
}

// kate: tab-width 4
