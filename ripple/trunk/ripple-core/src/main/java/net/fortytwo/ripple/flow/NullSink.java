/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.flow.Sink;

public class NullSink<T, E extends Exception> implements Sink<T, E>
{
	public void put( final T t ) throws E
	{
	}
}
