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
import net.fortytwo.ripple.util.Sink;

public class Cat extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Cat()
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
		RippleList l1, l2;

		l1 = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();
		l2 = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		RippleList result = RippleList.concat( l1, l2 );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
