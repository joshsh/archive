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
 * Consumes a list and an index n and produces the nth item in the list, where
 * the first item in the list has an index of 1.
 */
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
		RippleValue l;

		final int i = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		l = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( RippleList list ) throws RippleException
			{
				if ( i < 1 )
				{
					throw new RippleException( "list index out of bounds (note: 'at' begins counting at 1): " + i );
				}
		
				for ( int j = 1; j < i; j++ )
				{
					list = list.getRest();
					if ( RippleList.NIL == list )
					{
						throw new RippleException( "list index out of bounds: " + i );
					}
				}
		
				sink.put( new RippleList( list.getFirst(), rest ) );
			}
		};

		RippleList.from( l, listSink, mc );
	}
}

// kate: tab-width 4
