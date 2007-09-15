/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

public class Collector<T> implements Sink<T>, Source<T>
{
	private LinkedList<T> items = new LinkedList<T>();

	public Collector()
	{
		super();
	}

	public void put( final T t ) throws RippleException
	{
		items.add( t );
	}

	public void writeTo( final Sink<T> sink ) throws RippleException
	{
		Iterator<T> iter = items.iterator();
		while ( iter.hasNext() )
		{
			sink.put( iter.next() );
		}
	}

	public int size()
	{
		return items.size();
	}

	public Iterator<T> iterator()
	{
		return items.iterator();
	}

	public void clear()
	{
		items.clear();
	}
}

// kate: tab-width 4
