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
 * A primitive which consumes a number and produces its arc sine (if defined),
 * in the range of -pi/2 through pi/2.
 */
public class Asin extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Asin()
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
		double a;
		NumericLiteral result;

		a = mc.numericValue( stack.getFirst() ).doubleValue();
		stack = stack.getRest();

		// Apply the function only if it is defined for the given argument.
		if ( a >= -1 && a <= 1 )
		{
			result = new NumericLiteral( Math.asin( a ) );

			sink.put( new RippleList( result, stack ) );
		}
	}
}

// kate: tab-width 4
