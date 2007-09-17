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
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

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
		RippleValue f, v, l;

		f = stack.getFirst();
		stack = stack.getRest();
		v = stack.getFirst();
		stack = stack.getRest();
		l = stack.getFirst();
		stack = stack.getRest();

		RippleList lList = RippleList.invert( ( l instanceof RippleList )
			? (RippleList) l
			:  RippleList.createList( l.toRdf( mc ), mc ) );

		RippleList result = new RippleList( v, stack );

		while ( RippleList.NIL != lList )
		{
			result = result.push( lList.getFirst() )
				.push( f )
				.push( Operator.OP );
			lList = lList.getRest();
		}

		sink.put( result );
	}
}

// kate: tab-width 4
