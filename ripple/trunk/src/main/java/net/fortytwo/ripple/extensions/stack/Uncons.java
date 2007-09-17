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

public class Uncons extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Uncons()
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

		RippleList inv = RippleList.invert( l );
		RippleValue f = inv.getFirst();
		RippleList r = RippleList.invert( inv.getRest() );

		sink.put( new RippleList( f, stack ).push( r ) );
	}
}

// kate: tab-width 4
