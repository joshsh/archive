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
 * A primitive which consumes a string and produces its length.
 */
public class Length extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Length()
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
		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		sink.put( mc.list(
			mc.value( s.length() ), stack ) );
	}
}

// kate: tab-width 4
