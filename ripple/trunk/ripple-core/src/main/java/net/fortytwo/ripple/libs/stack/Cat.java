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
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive function which consumes two lists and produces the concatenation
 * of the two lists.
 */
public class Cat extends PrimitiveStackMapping
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		RippleValue l1, l2;

		l1 = stack.getFirst();
		stack = stack.getRest();
		l2 = stack.getFirst();
		final RippleList rest = stack.getRest();

		final Collector<RippleList> firstLists = new Collector<RippleList>();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( final RippleList list2 ) throws RippleException
			{
				Sink<RippleList> catSink = new Sink<RippleList>()
				{
					public void put( final RippleList list1 ) throws RippleException
					{
						RippleList result = mc.concat( list2, list1 );
						sink.put( arg.with(
								rest.push( result ) ) );
					}
				};

				firstLists.writeTo( catSink );
			}
		};

		mc.toList( l1, firstLists );
		mc.toList( l2, listSink );
	}
}

// kate: tab-width 4
