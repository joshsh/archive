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
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a list, an "initial value" and a filter, then
 * produces the result of folding the list with the given filter and initial
 * value.  For instance, <code>(1 2 3) 0 add /fold</code> yields 0 + 1 + 2 + 3
 * = 6.
 */
public class Fold extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public Fold()
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

		final RippleValue f = stack.getFirst();
		stack = stack.getRest();
		final RippleValue v = stack.getFirst();
		stack = stack.getRest();
		l = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( final RippleList list ) throws RippleException
			{
				RippleList lList = RippleList.invert( list );
				RippleList result = new RippleList( v, rest );
		
				while ( RippleList.NIL != lList )
				{
					result = result.push( lList.getFirst() )
						.push( f )
						.push( Operator.OP );
					lList = lList.getRest();
				}
		
				sink.put( result );
			}
		};

		RippleList.from( l, listSink, mc );
	}
}

// kate: tab-width 4
