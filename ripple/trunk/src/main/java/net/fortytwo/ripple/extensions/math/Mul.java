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
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Mul extends PrimitiveFunction
{
	public Mul()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 2;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		NumericLiteral a, b, result;

		b = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();
		a = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();

		result = NumericLiteral.mul( a, b );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
