/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.RdfValue;

/**
*  A function which reduces arbitrary expressions to a particular normal form.
*/
public abstract class Evaluator implements Function
{
	private RdfValue rdfEquivalent = null;

	public int arity()
	{
		// An Evaluator performs reduction; it does not need the stack to be
		// reduced to any level before it is received as input.
		return 0;
	}

	public abstract void stop();
}

// kate: tab-width 4
