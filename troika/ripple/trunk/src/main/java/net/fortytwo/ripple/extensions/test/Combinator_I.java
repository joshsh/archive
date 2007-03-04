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

public class Combinator_I extends Combinator
{
    public Combinator_I( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "I" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws WurfelException
    {
        sink.put( stack );
/*
        Value x;

        x = stack.getFirst();
        stack = stack.getRest();

        sink.put( stack
            .push( x ) );
*/
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
