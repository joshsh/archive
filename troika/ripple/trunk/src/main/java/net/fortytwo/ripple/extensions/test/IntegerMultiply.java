package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

public class IntegerMultiply extends PrimitiveFunction
{
    public IntegerMultiply( ModelConnection mc )
        throws RippleException
    {
        super( mc.createRippleTestUri( "integer-multiply" ), mc );
    }

    protected void applyInternal( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        int a, b, result;

        a = mc.intValue( stack.getFirst() );
        stack = stack.getRest();
        b = mc.intValue( stack.getFirst() );
        stack = stack.getRest();

        result = a * b;

        sink.put( new RippleStack( mc.createValue( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
