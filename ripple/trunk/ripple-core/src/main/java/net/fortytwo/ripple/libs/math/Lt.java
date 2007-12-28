/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.libs.stack.StackLibrary;

/**
 * A primitive which consumes two items x and y and produces a Boolean value of
 * true if x is less than y according to the natural ordering of x, otherwise
 * false.
 */
public class Lt extends PrimitiveFunction
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

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue a, b, result;

		b = stack.getFirst();
		stack = stack.getRest();
		a = stack.getFirst();
		stack = stack.getRest();

		result = ( a.compareTo( b ) < 0 )
			? StackLibrary.getTrueValue()
			: StackLibrary.getFalseValue();

		sink.put( stack.push( result ) );
	}
}

// kate: tab-width 4
