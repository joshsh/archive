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

public interface StackRelation extends BinaryRelation<StackContext>
{
	/**
	*  The fixed number of arguments which this function consumes before
	*  yielding a result.
	*/
	int arity();
}
