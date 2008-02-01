/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a Boolean value and produces its inverse.
 */
public class Not extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Not()
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
		RippleValue x;

		x = stack.getFirst();
		stack = stack.getRest();

		// Note: everything apart from joy:true is considered false.
		RippleValue result = ( 0 == x.compareTo( StackLibrary.getTrueValue() ) )
			? StackLibrary.getFalseValue()
			: StackLibrary.getTrueValue();

		sink.put( stack.push( result ) );
	}
}

// kate: tab-width 4
