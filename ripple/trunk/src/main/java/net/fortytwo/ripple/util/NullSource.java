package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class NullSource<T> implements Source<T>
{
	public void writeTo( Sink<T> sink ) throws RippleException
	{
	}
}

// kate: tab-width 4
