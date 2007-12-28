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
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes an item and a number n, then pushes n active
 * copies of the item to the stack.  This has the effect of applying the
 * filter "n times" to the remainder of the stack.
 */
public class Times extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Times()
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
		int times;

		times = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		RippleValue p = stack.getFirst();
		stack = stack.getRest();

		if ( times < 0 )
		{
			throw new RippleException(
				"first argument to Times must be a positive integer" );
		}

		for ( int i = 0; i < times; i++ )
		{
			stack = mc.list( p, stack ).push( Operator.OP );
		}

		sink.put( stack );
	}
}

// kate: tab-width 4
