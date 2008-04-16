/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

public class Buffer<T, E extends Exception> extends Collector<T, E>
{
	private Sink<T, E> sink;

	public Buffer( final Sink<T, E> sink )
	{
		super();

		this.sink = sink;
	}

	public void flush() throws E
	{
		writeTo( sink );

		clear();
	}
}
