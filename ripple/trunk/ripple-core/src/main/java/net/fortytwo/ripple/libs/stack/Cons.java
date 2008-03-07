/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes an item and a list, prepends the item to the list,
 * then produces the resulting list.
 */
public class Cons extends PrimitiveStackMapping
{
	private static final int ARITY = 2;

	public Cons()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		RippleValue l;

		l = stack.getFirst();
		stack = stack.getRest();
		final RippleValue x = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( final RippleList list ) throws RippleException
			{
				sink.put( arg.with(
						rest.push( list.push( x ) ) ) );
			}
		};

		mc.toList( l, listSink );
	}
}

// kate: tab-width 4
