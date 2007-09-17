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
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

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
		RippleList prog;

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
			stack = new RippleList( p, stack ).push( Operator.OP );
		}

		sink.put( stack );
	}
}

// kate: tab-width 4
