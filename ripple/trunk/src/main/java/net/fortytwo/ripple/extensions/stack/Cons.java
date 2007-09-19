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

public class Cons extends PrimitiveFunction
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

	private RippleList bury( final RippleValue last, final RippleList l )
	{
		if ( RippleList.NIL == l )
		{
			return new RippleList( last );
		}

		else
		{
			return new RippleList( l.getFirst(), bury( last, l.getRest() ) );
		}
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue l;

		l = stack.getFirst();
		stack = stack.getRest();
		final RippleValue x = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( final RippleList list ) throws RippleException
			{
				sink.put( new RippleList( bury( x, list ), rest ) );
			}
		};

		RippleList.from( l, listSink, mc );
	}
}

// kate: tab-width 4
