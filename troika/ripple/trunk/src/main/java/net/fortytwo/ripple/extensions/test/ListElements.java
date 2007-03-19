package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.util.Iterator;

public class ListElements extends PrimitiveFunction
{
    public ListElements( RdfValue v, ModelConnection mc )
        throws RippleException
    {
        super( v, mc );
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
        Iterator<RippleValue> values;

        values = mc.listValue( stack.getFirst() ).iterator();
        stack = stack.getRest();

        while ( values.hasNext() )
            sink.put( new RippleStack( values.next(), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
