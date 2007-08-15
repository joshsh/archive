/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public interface Sink<T>
{
	public void put( T t ) throws RippleException;
}

// kate: tab-width 4
