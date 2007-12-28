/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

public class ListDequotation implements Function
{
	private RippleList list;

	public ListDequotation( final RippleList list )
	{
		this.list = list;
	}

	public int arity()
	{
		return 0;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleList in = list;
		RippleList out = stack;

		while ( RippleList.NIL != in )
		{
			out = mc.list( in.getFirst(), out );
			in = in.getRest();
		}

		// Never emit an empty stack.
		if ( RippleList.NIL != out )
		{
			sink.put( out );
		}
	}
}

// kate: tab-width 4
