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

public class Swons extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Swons()
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
		RippleValue x;
		RippleList l;

		x = stack.getFirst();
		stack = stack.getRest();
		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		sink.put( new RippleList( bury( x, l ), stack ) );
	}
}

// kate: tab-width 4
