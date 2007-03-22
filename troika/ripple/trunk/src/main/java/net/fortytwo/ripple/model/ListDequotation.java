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

	public boolean isOperator()
	{
		return false;
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

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
p.print( reversedList );
    }

public RdfValue toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
