/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.*;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A primitive which optionally activates ("applies") the topmost item on the
 * stack.
 */
public class IOpt extends PrimitiveFunction
{
// Arguably 0...
	private static final int ARITY = 1;

	public IOpt() throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue first = stack.getFirst();

// hack...
		sink.put( mc.list( Operator.OP, stack ) );

		stack = stack.getRest();
		sink.put(stack);
	}
}