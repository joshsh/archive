package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public interface Source<T>
{
	public void writeTo( Sink<T> sink ) throws RippleException;
}

// kate: tab-width 4
