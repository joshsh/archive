/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a string, strips off all leading and trailing
 * white space, and produces the result.
 */
public class Trim extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Trim()
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
		String s, result;

		s = mc.toString( stack.getFirst() );
		stack = stack.getRest();

		result = s.trim();

		sink.put( mc.list( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
