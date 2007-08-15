package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class ListDequotation implements Function
{
	RippleList reversedList;

	private static RippleList reverse( final RippleList list )
	{
		RippleList in = list;
		RippleList out = null;
		while ( null != in )
		{
			out = new RippleList( in.getFirst(), out );
			in = in.getRest();
		}

		return out;
	}

	public ListDequotation( RippleList list )
	{
		reversedList = reverse( list );
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

		while ( null != in )
		{
			out = new RippleList( in.getFirst(), out );
			in = in.getRest();
		}

		sink.put( out );
	}
}

// kate: tab-width 4
