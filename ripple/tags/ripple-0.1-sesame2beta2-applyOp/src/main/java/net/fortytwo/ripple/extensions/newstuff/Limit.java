package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Limit extends PrimitiveFunction
{
    public Limit( ModelConnection mc )
        throws RippleException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "limit" ), mc );
    }

    public void applyInternal( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        int lim;

        lim = mc.intValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

        sink.put( new ListNode<Value>( new net.fortytwo.ripple.model.filter.Limit( (long) lim ), stack )
            .push( mc.getApplyOp() ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
