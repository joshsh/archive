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

public class Signum extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Signum()
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
		NumericLiteral a, result;

		a = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();

		result = new NumericLiteral( a.sign() );

		sink.put(
			new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
