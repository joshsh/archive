package net.fortytwo.ripple.model.filter;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Limit extends Filter
{
    long count, limit;

    public Limit( long lim )
    {
        limit = lim;
        count = 0;
//System.out.println( "" + this + "()" );
    }

    public void applyTo( RippleStack stack,
                         Sink<RippleStack> sink,
                         ModelConnection mc )
        throws RippleException
    {
        if ( count < limit )
        {
            count++;
            sink.put( stack );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
