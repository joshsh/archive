/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a number and produces all real square roots of the
 * number.
 */
public class Sqrt extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Sqrt()
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
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		double a;

		a = mc.toNumericValue( stack.getFirst() ).doubleValue();
		stack = stack.getRest();

		// Apply the function only if it is defined for the given argument.
		if ( a >= 0 )
		{
			double d = Math.sqrt( a );

			// Yield both square roots.
			sink.put( mc.list( mc.value( d ), stack ) );
			if ( d > 0 )
			{
				sink.put( mc.list( mc.value( 0.0 - d ), stack ) );
			}
		}
	}
}

// kate: tab-width 4
