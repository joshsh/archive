package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.net.URLEncoder;

public class UrlEncoding extends PrimitiveFunction
{
    public UrlEncoding( RdfValue v, ModelConnection mc )
        throws RippleException
    {
        super( v, mc );
    }

    public void applyTo( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        String a, result;

        a = mc.stringValue( stack.getFirst() );
        stack = stack.getRest();

        try
        {
            result = URLEncoder.encode( a );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        sink.put( new RippleStack( mc.createValue( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
