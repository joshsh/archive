package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class ConcatenateStringsPrimitive extends PrimitiveFunction
{
    public ConcatenateStringsPrimitive( RdfValue v, ModelConnection mc )
        throws RippleException
    {
        super( v, mc );
    }

    public void applyTo( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        String strA, strB, result;

        strA = mc.stringValue( stack.getFirst() );
        stack = stack.getRest();
        strB = mc.stringValue( stack.getFirst() );
        stack = stack.getRest();

        result = strA + strB;

        sink.put( new RippleStack( mc.createValue( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
