/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

public class CollectorHistory<T> implements Sink<T>
{
	private int len, maxlen;
	private LinkedList<Collector<T>> history;
	private Sink<T> currentSink;

	public CollectorHistory( final int maxlen )
	{
		if ( maxlen < 1 )
		{
			throw new IllegalArgumentException();
		}

		len = 0;
		this.maxlen = maxlen;
		history = new LinkedList<Collector<T>>();

		advance();
	}

	public void advance()
	{

		Collector<T> coll = new Collector<T>();
		history.addFirst( coll );

		len++;
		if ( len > maxlen )
		{
			history.removeLast();
		}

		currentSink = coll;
	}

	public void put( final T t )
		throws RippleException
	{
		currentSink.put( t );
	}

	public Source<T> getSource( final int index )
		throws RippleException
	{
		return history.get( index );
	}
}

// kate: tab-width 4
