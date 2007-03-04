package net.fortytwo.ripple.extensions.newstuff;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Dup extends Combinator
{
    public Dup( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "dup" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws WurfelException
    {
        Value x;

        x = stack.getFirst();
        stack = stack.getRest();

        sink.put( new ListNode<Value>( x, stack ).push( x ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
