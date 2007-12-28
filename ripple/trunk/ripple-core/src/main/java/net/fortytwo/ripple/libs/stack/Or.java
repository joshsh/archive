/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes two Boolean values and produces the result of
 * their inclusive logical disjunction.
 */
public class Or extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Or()
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
		RippleValue x, y;

		x = stack.getFirst();
		stack = stack.getRest();
		y = stack.getFirst();
		stack = stack.getRest();

		RippleValue trueValue = StackLibrary.getTrueValue();

		// Note: everything apart from joy:true is considered false.
		RippleValue result = ( 0 == x.compareTo( trueValue ) || 0 == y.compareTo( trueValue ) )
			? trueValue
			: StackLibrary.getFalseValue();

		sink.put( mc.list( result, stack ) );
	}
}

// kate: tab-width 4
