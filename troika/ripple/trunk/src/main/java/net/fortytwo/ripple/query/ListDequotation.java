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
	RippleList reversedList;

	public ListDequotation( RippleList list )
	{
		reversedList = RippleList.invert( list );
	}

	public int arity()
	{
		return 0;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleList in = reversedList;
		RippleList out = stack;

		while ( RippleList.NIL != in )
		{
			out = new RippleList( in.getFirst(), out );
			in = in.getRest();
		}

		// Never emit an empty stack.
		if ( RippleList.NIL != out )
			sink.put( out );
	}
}

// kate: tab-width 4
