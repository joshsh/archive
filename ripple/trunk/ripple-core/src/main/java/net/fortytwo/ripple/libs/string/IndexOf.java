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
 * A primitive which consumes a string and a substring and produces the index of
 * the first occurrence of the substring.
 */
public class IndexOf extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public IndexOf()
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
		String str, substr;
		int result;

		substr = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		str = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = str.indexOf( substr );
		sink.put( mc.list( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
