/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import java.util.HashSet;
import java.util.Set;

public class UniqueFilter<T, E extends Exception> implements Sink<T, E>
{
	private Set<T> set = new HashSet<T>();
	private Sink<T, E> sink;

	public UniqueFilter( final Sink<T, E> sink )
	{
		this.sink = sink;
	}

	public void put( final T t ) throws E
	{
		if ( set.add( t ) )
		{
			sink.put( t );
		}
	}
}
