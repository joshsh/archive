/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

/**
 *  A filter which yields no results, regardless of its inputs.
 */
public class NullFilter implements Function
{
	public int arity()
	{
		return 0;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final Context context )
		throws RippleException
	{
	}
	
	public boolean isTransparent()
	{
		return true;
	}
}

// kate: tab-width 4
