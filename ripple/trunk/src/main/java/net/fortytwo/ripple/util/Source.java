/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public interface Source<T>
{
	public void writeTo( Sink<T> sink ) throws RippleException;
}

// kate: tab-width 4
