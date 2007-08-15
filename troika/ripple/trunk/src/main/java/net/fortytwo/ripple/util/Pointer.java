/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

public class Pointer<T>
{
	public T ref;

	public Pointer()
	{
		ref = null;
	}

	public Pointer( final T ref )
	{
		this.ref = ref;
	}
}

// kate: tab-width 4
