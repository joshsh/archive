/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which pushes a copy of the topmost item on the stack to the
 * head of the stack.
 */
public class Dup extends PrimitiveStackMapping
{
	private static final int ARITY = 1;

	public Dup()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		RippleValue x;
		RippleList stack = arg.getStack();

		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( arg.with(
				stack.push( x ).push( x ) ) );
	}
}

// kate: tab-width 4
