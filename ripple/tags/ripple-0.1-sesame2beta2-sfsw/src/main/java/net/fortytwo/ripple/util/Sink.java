package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public interface Sink<T>
{
	public void put( T t ) throws RippleException;
}

// kate: tab-width 4
