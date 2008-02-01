/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a list and produces the greatest item in the list.
 */
public class Max extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Max()
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
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		RippleValue l;

		l = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleList> listSink = new Sink<RippleList>()
		{
			public void put( RippleList list ) throws RippleException
			{
				RippleValue result = null;
				while ( RippleList.NIL != list )
				{
					RippleValue v = list.getFirst();
		
					if ( null == result || v.compareTo( result ) > 0 )
					{
						result = v;
					}
		
					list = list.getRest();
				}
		
				if ( null != result )
				{
					sink.put( rest.push( result ) );
				}
			}
		};

		mc.toList( l, listSink );
	}
}

// kate: tab-width 4
