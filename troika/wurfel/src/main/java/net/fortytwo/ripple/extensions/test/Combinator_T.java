package net.fortytwo.ripple.extensions.test;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.Apply;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_T extends Combinator
{
    public Combinator_T( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "T" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws WurfelException
    {
        Value x, y;

        x = stack.getFirst();
        stack = stack.getRest();
        y = stack.getFirst();
        stack = stack.getRest();

        sink.put( new ListNode<Value>( x, stack )
            .push( y )
            .push( mc.getApplyOp() ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
