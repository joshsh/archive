/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;

/**
 *  A filter which yields no results, regardless of its inputs.
 */
public class NullFilter implements StackMapping
{
	public int arity()
	{
		return 0;
	}

	public void applyTo( final StackContext arg,
						final Sink<StackContext, RippleException> sink )
		throws RippleException
	{
	}
	
	public boolean isTransparent()
	{
		return true;
	}
}

// kate: tab-width 4
