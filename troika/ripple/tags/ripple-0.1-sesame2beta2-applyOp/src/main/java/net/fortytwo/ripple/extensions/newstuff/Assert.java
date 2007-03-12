package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Assert extends Combinator
{
    public Assert( ModelConnection mc )
        throws RippleException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "assert" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        Value subj, pred, obj;

        obj = stack.getFirst();
        stack = stack.getRest();
        pred = stack.getFirst();
        stack = stack.getRest();
        subj = stack.getFirst();
        stack = stack.getRest();

        mc.add( subj, pred, obj );

        sink.put( new ListNode<Value>( subj, stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
