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

public class X extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public X()
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
		RippleValue x;

		x = stack.getFirst();

		sink.put( new RippleList( x, stack ).push( Operator.OP ) );
	}
}

// kate: tab-width 4
