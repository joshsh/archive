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
 * A primitive which consumes two numbers x and y and produces the number x
 * modulo y.
 */
public class Mod extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Mod()
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
		NumericLiteral a, b, result;

		b = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();
		a = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();

		// Note: mod by zero simply does not yield a result.
		if ( !b.isZero() )
		{
			result = NumericLiteral.mod( a, b );

			sink.put( new RippleList( result, stack ) );
		}
	}
}

// kate: tab-width 4
