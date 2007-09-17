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

public class In extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public In()
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
		RippleValue x;
		RippleList l;

		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();
		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( has( l, x ) ? StackExtension.getTrueValue() : StackExtension.getFalseValue(), stack ) );
	}
}

// kate: tab-width 4
