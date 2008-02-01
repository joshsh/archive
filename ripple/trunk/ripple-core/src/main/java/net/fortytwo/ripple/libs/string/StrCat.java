/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes two strings and produces their
 * concatenation.
 */
public class StrCat extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public StrCat()
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

		String strA, strB, result;

		strA = mc.toString( stack.getFirst() );
		stack = stack.getRest();
		strB = mc.toString( stack.getFirst() );
		stack = stack.getRest();

		result = strB + strA;

		sink.put( stack.push( mc.value( result ) ) );
	}
}

// kate: tab-width 4
