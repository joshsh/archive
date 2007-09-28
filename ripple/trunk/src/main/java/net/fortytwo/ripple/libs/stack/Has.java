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
				sink.put( new RippleList( has( list, x ) ? StackLibrary.getTrueValue() : StackLibrary.getFalseValue(), rest ) );
			}
		};

		RippleList.from( l, listSink, mc );
	}
}

// kate: tab-width 4
