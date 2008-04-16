/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import java.util.LinkedList;

public class CollectorHistory<T, E extends Exception> implements Sink<T, E>
{
	private int len, maxlen;
	private LinkedList<Collector<T, E>> history;
	private Sink<T, E> currentSink;

	public CollectorHistory( final int maxlen )
	{
		if ( maxlen < 1 )
		{
			throw new IllegalArgumentException();
		}

		len = 0;
		this.maxlen = maxlen;
		history = new LinkedList<Collector<T, E>>();

		advance();
	}

	public void advance()
	{
		Collector<T, E> coll = new Collector<T, E>();
		history.addFirst( coll );

		len++;
		if ( len > maxlen )
		{
			history.removeLast();
		}

		currentSink = coll;
	}

	public void put( final T t ) throws E
	{
		currentSink.put( t );
	}

	public Source<T, E> getSource( final int index ) throws E
	{
		try
		{
			return history.get( index );
		}

		catch ( IndexOutOfBoundsException e )
		{
			throw e;
		}
	}
}
