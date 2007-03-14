package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class UriToString extends PrimitiveFunction
{
    public UriToString( ModelConnection mc )
        throws RippleException
    {
        super( mc.createRippleMiscUri( "uriToString" ), mc );
    }

    protected void applyInternal( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        URI a;
        String result;

        a = mc.castToUri( stack.getFirst() );
        stack = stack.getRest();

        try
        {
            result = a.toString();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        sink.put( new RippleStack( mc.createLiteral( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
