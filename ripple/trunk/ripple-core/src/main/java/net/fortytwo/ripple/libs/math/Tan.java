/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.NumericValue;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a number representing an angle in radians and
 * produces its tangent.
 */
public class Tan extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Tan()
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
		NumericValue result;

		a = mc.toNumericValue( stack.getFirst() ).doubleValue();
		stack = stack.getRest();

// TODO: check for undefined values
		double d = Math.tan( a );
		result = mc.value( d );

		sink.put( mc.list( result, stack ) );
	}
}

// kate: tab-width 4
