package net.fortytwo.ripple.util;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

public class Buffer<T> extends Collector<T>
{
	private Sink<T> sink;

	public Buffer( Sink<T> sink )
	{
		super();

		this.sink = sink;
	}

	public void flush() throws RippleException
	{
		for ( Iterator<T> i = iterator(); i.hasNext(); )
			sink.put( i.next() );

		clear();
	}
}

// kate: tab-width 4
