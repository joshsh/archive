package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerMod extends PrimitiveFunction
{
    public IntegerMod( RdfValue v, ModelConnection mc )
        throws RippleException
    {
        super( v, mc );
    }

	public int arity()
	{
		return 2;
	}

    public void applyTo( RippleList stack,
                                  Sink<RippleList> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        int a, b, result;

        a = mc.intValue( stack.getFirst() );
        stack = stack.getRest();
        b = mc.intValue( stack.getFirst() );
        stack = stack.getRest();

        try
        {
            result = a % b;
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        sink.put( new RippleList( mc.createValue( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
