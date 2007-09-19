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
import net.fortytwo.ripple.util.Collector;
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
						RippleList result = RippleList.concat( list1, list2 );
						sink.put( new RippleList( result, rest ) );
					}
				};

				firstLists.writeTo( catSink );
			}
		};

		RippleList.from( l1, firstLists, mc );
		RippleList.from( l2, listSink, mc );
	}
}

// kate: tab-width 4
