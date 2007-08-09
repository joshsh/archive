package net.fortytwo.ripple.util;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

public class Collector<T> extends LinkedList<T> implements Sink<T>, Source<T>
{
	public Collector()
	{
		super();
	}

	public void put( final T t ) throws RippleException
	{
		add( t );
	}

	public void writeTo( final Sink<T> sink ) throws RippleException
	{
		Iterator<T> iter = iterator();
		while ( iter.hasNext() )
			sink.put( iter.next() );
	}
}

// kate: tab-width 4
