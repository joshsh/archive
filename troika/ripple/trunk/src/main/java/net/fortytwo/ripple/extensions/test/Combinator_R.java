package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;
import net.fortytwo.ripple.model.FunctionEnvelope;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_R extends Combinator
{
    public Combinator_R( ModelConnection mc )
        throws RippleException
    {
        super( mc.createRippleTestUri( "R" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        Value x, y, z;

        x = stack.getFirst();
        stack = stack.getRest();
        y = stack.getFirst();
        stack = stack.getRest();
        z = stack.getFirst();
        stack = stack.getRest();

        sink.put( new ListNode<Value>( x, stack )
            .push( z )
            .push( y )
            .push( mc.getApplyOp() )
            .push( mc.getApplyOp() ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
