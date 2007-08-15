package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Swap extends Combinator
{
    public Swap( ModelConnection mc )
        throws RippleException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "swap" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        Value x, y;

        x = stack.getFirst();
        stack = stack.getRest();
        y = stack.getFirst();
        stack = stack.getRest();

        sink.put( new ListNode<Value>( x, stack ).push ( y ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
