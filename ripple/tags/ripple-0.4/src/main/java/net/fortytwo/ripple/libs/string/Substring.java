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
 * A primitive which consumes a string and two integer indexes, then
 * produces the substring between the first index (inclusive) and the second
 * index (exclusive).
 */
public class Substring extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public Substring()
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
		int begin, end;
		String s, result;

		end = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		begin = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			result = s.substring( begin, end );
			sink.put( new RippleList( mc.createValue( result ), stack ) );
		}

		catch ( IndexOutOfBoundsException e )
		{
			// Silent fail.
			return;
		}
	}
}

// kate: tab-width 4
