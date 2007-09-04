/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class NullSink<T> implements Sink<T>
{
	public void put( final T t ) throws RippleException
	{}
}

// kate: tab-width 4
