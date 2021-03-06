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
 * A primitive which activates the second-to-topmost item on the stack.
 */
public class Dip extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Dip()
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
		RippleValue y, x;

// hack...
		y = stack.getFirst();
		stack = stack.getRest();
		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( y, stack ).push( Operator.OP ).push( x ) );
	}
}

// kate: tab-width 4
