/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.libs.stack.StackLibrary;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes two items x and y and produces a Boolean value of
 * true if x is less than y according to the natural ordering of x, otherwise
 * false.
 */
public class Lt extends PrimitiveStackMapping
{
	private static final int ARITY = 2;

	public Lt()
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
		RippleValue a, b, result;
		RippleList stack = arg.getStack();

		b = stack.getFirst();
		stack = stack.getRest();
		a = stack.getFirst();
		stack = stack.getRest();

		result = ( a.compareTo( b ) < 0 )
			? StackLibrary.getTrueValue()
			: StackLibrary.getFalseValue();

		sink.put( arg.with(
				stack.push( result ) ) );
	}
}

// kate: tab-width 4
