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

public interface Function
{
	/**
	*  The fixed number of arguments which this function consumes before
	*  yielding a result.
	*/
	int arity();

	void applyTo( RippleList stack,
					Sink<RippleList> sink,
					ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
