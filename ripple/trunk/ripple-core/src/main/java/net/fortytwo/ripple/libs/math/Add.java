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
 * A primitive which consumes two numbers and produces their sum.
 */
public class Add extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Add()
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

		NumericValue a, b, result;

		b = mc.toNumericValue( stack.getFirst() );
		stack = stack.getRest();
		a = mc.toNumericValue( stack.getFirst() );
		stack = stack.getRest();

		result = a.add( b );

		sink.put( mc.list( result, stack ) );
	}
}

// kate: tab-width 4
