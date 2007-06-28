package net.fortytwo.ripple.util;

import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

public class Collector<T> extends LinkedList<T> implements Sink<T>
{
	public Collector()
	{
		super();
	}

	public void put( T t ) throws RippleException
	{
		add( t );
	}
}

// kate: tab-width 4
