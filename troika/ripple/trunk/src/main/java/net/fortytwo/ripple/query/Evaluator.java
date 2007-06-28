package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;

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
}

// kate: tab-width 4
