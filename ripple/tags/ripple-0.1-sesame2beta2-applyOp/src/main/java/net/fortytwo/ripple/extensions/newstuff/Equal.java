package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Equal extends Combinator
{
    public Equal( ModelConnection mc )
        throws RippleException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "equal" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        Value a, b;
        Combinator result;

        a = stack.getFirst();
        stack = stack.getRest();
        b = stack.getFirst();
        stack = stack.getRest();

// TODO: we shouldn't need to create a new Combinator instance for each result.
        result = ( a.equals( b ) )
            ? new True( mc )
            : new False( mc );

        sink.put( new ListNode<Value>( result, stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
