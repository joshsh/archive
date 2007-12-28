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
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a list and an item and produces a Boolean value of
 * true if the item is contained in the list, otherwise false.
 */
public class Has extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Has()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	private boolean has( RippleList l, final RippleValue v )
		throws RippleException
	{
		while ( RippleList.NIL != l )
		{
			if ( 0 == l.getFirst().compareTo( v ) )
			{
				return true;
			}

			l = l.getRest();
		}

		return false;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue l;

		final RippleValue x = stack.getFirst();
		stack = stack.getRest();
		l = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( final RippleList list ) throws RippleException
			{
				sink.put( mc.list( has( list, x ) ? StackLibrary.getTrueValue() : StackLibrary.getFalseValue(), rest ) );
			}
		};

		mc.toList( l, listSink );
	}
}

// kate: tab-width 4
