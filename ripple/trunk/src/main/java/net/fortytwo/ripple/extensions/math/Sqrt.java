/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.NumericLiteral;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

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
						final ModelConnection mc )
		throws RippleException
	{
		double a;

		a = mc.numericValue( stack.getFirst() ).doubleValue();
		stack = stack.getRest();

		// Apply the function only if it is defined for the given argument.
		if ( a >= 0 )
		{
			double d = Math.sqrt( a );

			// Yield both square roots.
			sink.put( new RippleList( new NumericLiteral( d ), stack ) );
			if ( d > 0 )
			{
				sink.put( new RippleList( new NumericLiteral( 0.0 - d ), stack ) );
			}
		}
	}
}

// kate: tab-width 4
