package net.fortytwo.ripple.extensions.intmath;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerMultiply extends PrimitiveFunction
{
    public IntegerMultiply( RdfValue v, ModelConnection mc )
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

        b = mc.intValue( stack.getFirst() );
        stack = stack.getRest();
        a = mc.intValue( stack.getFirst() );
        stack = stack.getRest();

        result = a * b;

        sink.put( new RippleList( mc.createValue( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
