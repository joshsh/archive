/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a list and produces each item in the list in a
 * separate stack.
 */
public class Each extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Each()
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
		RippleValue l;

		l = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( RippleList list ) throws RippleException
			{
				while ( RippleList.NIL != list )
				{
					sink.put( mc.list( list.getFirst(), rest ) );
					list = list.getRest();
				}
			}
		};

		mc.toList( l, listSink );
	}
}

// kate: tab-width 4
