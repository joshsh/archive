/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class Switch<T> implements Sink<T>
{
	private Sink<T> left, right;
	private boolean state;

	public Switch( final Sink<T> left, final Sink<T> right )
	{
		this.left = left;
		this.right = right;
		state = true;
	}

	public void put( final T t ) throws RippleException
	{
		( ( state ) ? left : right ).put( t );
	}

	public void flip()
	{
		state = !state;
	}
}

// kate: tab-width 4
