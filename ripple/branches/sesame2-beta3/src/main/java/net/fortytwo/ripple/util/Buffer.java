package net.fortytwo.ripple.util;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

public class Buffer<T> implements Sink<T>
{
	private Sink<T> sink;
	Collection<T> collection;

	public Buffer( Sink<T> sink )
	{
		this.sink = sink;
		collection = new LinkedList<T>();
	}

	public void put( T t ) throws RippleException
	{
		collection.add( t );
	}

	public void flush() throws RippleException
	{
		for ( Iterator<T> i = collection.iterator(); i.hasNext(); )
			sink.put( i.next() );

		collection.clear();
	}
}

// kate: tab-width 4
