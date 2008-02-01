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
 * A primitive which consumes a string, maps its characters to lower case, and
 * produces the result.
 */
public class ToLowerCase extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public ToLowerCase()
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

		String s, result;

		s = mc.toString( stack.getFirst() );
		stack = stack.getRest();
		result = s.toLowerCase();

		sink.put( stack.push( mc.value( result ) ) );
	}
}

// kate: tab-width 4
