package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.ListMemoizer;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Unique extends Combinator
{
    private static String memo = "memo";

    private ListMemoizer<Value,String> memoizer = null;

    public Unique( ModelConnection mc )
        throws RippleException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "unique" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        if ( null == memoizer )
        {
            memoizer = new ListMemoizer<Value,String>( stack, memo );
            sink.put( stack );
        }

        else if ( memoizer.add( stack, memo ) )
            sink.put( stack );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
