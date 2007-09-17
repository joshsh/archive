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
import net.fortytwo.ripple.util.Sink;

public class At extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public At()
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
		int i;
		RippleList l;

		i = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		l = RippleList.invert( RippleList.from( stack.getFirst(), mc ) );
		stack = stack.getRest();

		if ( i < 1 )
		{
			throw new RippleException( "list index out of bounds (keep in mind that 'at' begins counting at 1): " + i );
		}

		for ( int j = 1; j < i; j++ )
		{
			l = l.getRest();
			if ( RippleList.NIL == l )
			{
				throw new RippleException( "list index out of bounds: " + i );
			}
		}

		sink.put( new RippleList( l.getFirst(), stack ) );
	}
}

// kate: tab-width 4
