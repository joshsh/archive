package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class NullSink<T> implements Sink<T>
{
	public void put( T t )
		throws RippleException
	{}
}

// kate: tab-width 4
