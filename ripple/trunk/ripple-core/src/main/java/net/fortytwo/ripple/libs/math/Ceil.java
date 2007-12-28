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
 * A primitive which consumes a number and produces the smallest integer value
 * which is greater than or equal to the number.  Note: the produced value has
 * a type of xsd:integer, unlike Java's Math.ceil, which returns a double
 * value.
 */
public class Ceil extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Ceil()
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

		a = mc.toNumericValue( stack.getFirst() );
		stack = stack.getRest();

		result = mc.value( (int) Math.ceil( a.doubleValue() ) );

		sink.put( mc.list( result, stack ) );
	}
}

// kate: tab-width 4
