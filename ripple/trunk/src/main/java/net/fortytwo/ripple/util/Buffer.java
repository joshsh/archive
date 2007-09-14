/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class Buffer<T> extends Collector<T>
{
	Sink<T> sink;

	public Buffer( Sink<T> sink )
	{
		super();

		this.sink = sink;
	}

	public void flush() throws RippleException
	{
		writeTo( sink );

		clear();
	}
}

// kate: tab-width 4
