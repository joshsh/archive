package net.fortytwo.ripple.model;

import java.util.Collection;

import org.openrdf.model.Value;

import net.fortytwo.ripple.RippleException;

public class ListContainerSink extends Container implements Sink<ListNode<Value>>
{
    public ListContainerSink()
    {
        super();
    }

    public void put( ListNode<Value> v )
        throws RippleException
    {
//System.out.println( this + ".put( " + v + " )" );
//System.out.flush();
        add( v );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
