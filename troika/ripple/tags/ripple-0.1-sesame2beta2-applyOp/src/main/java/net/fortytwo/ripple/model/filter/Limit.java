package net.fortytwo.ripple.model.filter;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ListMemoizer;
import net.fortytwo.ripple.model.ListNode;
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

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
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
