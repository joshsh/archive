/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which removes the second-to-topmost item from the stack.
 */
public class Popd extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Popd()
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
						final Context context )
		throws RippleException
	{
		RippleValue y;

		y = stack.getFirst();
		stack = stack.getRest();
		stack = stack.getRest();

		sink.put( stack.push( y ) );
	}
}

// kate: tab-width 4
