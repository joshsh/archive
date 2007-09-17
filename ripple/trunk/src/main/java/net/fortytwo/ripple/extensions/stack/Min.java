/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Min extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Min()
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
		RippleList l;

		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		RippleValue result = null;
		while ( RippleList.NIL != l )
		{
			RippleValue v = l.getFirst();
			if ( null == result || v.compareTo( result ) < 0 )
			{
				result = v;
			}

			l = l.getRest();
		}

		if ( null != result )
		{
			sink.put( new RippleList( result, stack ) );
		}
	}
}

// kate: tab-width 4
