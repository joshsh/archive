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
						final ModelConnection mc )
		throws RippleException
	{
		String strA, strB, result;

		strA = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		strB = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = strB + strA;

		sink.put( new RippleList( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
