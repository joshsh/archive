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
import net.fortytwo.ripple.model.NumericLiteral;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes two numbers x and y and produces the number x to
 * the power of y.
 */
public class Pow extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Pow()
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
		NumericLiteral p, x, result;

		p = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();
		x = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();

		result = x.pow( p );

		sink.put( mc.list( result, stack ) );
	}
}

// kate: tab-width 4
