package net.fortytwo.ripple.model;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class ListDequotation implements Function
{
    RippleStack reversedList;

    private static RippleStack reverse( final RippleStack list )
    {
        RippleStack in = list;
        RippleStack out = null;
        while ( null != in )
        {
            out = new RippleStack( in.getFirst(), out );
            in = in.getRest();
        }

        return out;
    }

    public ListDequotation( RippleStack list )
    {
        reversedList = reverse( list );
    }

    public int arity()
    {
return 1;
    }

    public void applyTo( RippleStack stack,
                         Sink<RippleStack> sink,
                         ModelConnection mc )
        throws RippleException
    {
        RippleStack in = reversedList;
        RippleStack out = stack;

        while ( null != in )
        {
            out = new RippleStack( in.getFirst(), out );
            in = in.getRest();
        }

        sink.put( out );
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
p.print( reversedList );
    }

public URI getUri()
{
return null;
}

public void checkArguments( RippleStack args )
    throws RippleException
{}

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
