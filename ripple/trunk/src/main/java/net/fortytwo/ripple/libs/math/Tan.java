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
						final ModelConnection mc )
		throws RippleException
	{
		double a;
		NumericLiteral result;

		a = mc.numericValue( stack.getFirst() ).doubleValue();
		stack = stack.getRest();

// TODO: check for undefined values
		double d = Math.tan( a );
		result = new NumericLiteral( d );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
