/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveStackRelation;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

/**
 * Consumes an index n and a list and produces the nth item in the list, where
 * the first item in the list has an index of 1.
 */
public class Of extends PrimitiveStackRelation
{
	private static final int ARITY = 2;

	public Of()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		RippleList stack = arg.getStack();
		final ModelConnection mc = arg.getModelConnection();

		RippleValue l;

		l = stack.getFirst();
		stack = stack.getRest();
		final int i = mc.toNumericValue( stack.getFirst() ).intValue();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( RippleList list ) throws RippleException
			{
				if ( i < 1 )
				{
					throw new RippleException( "list index out of bounds (keep in mind that 'at' begins counting at 1): " + i );
				}
		
				for ( int j = 1; j < i; j++ )
				{
					list = list.getRest();
					if ( RippleList.NIL == list )
					{
						throw new RippleException( "list index out of bounds: " + i );
					}
				}
		
				sink.put( arg.with(
						rest.push( list.getFirst() ) ) );
			}
		};

		mc.toList( l, listSink );
	}
}

// kate: tab-width 4
