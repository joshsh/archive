package net.fortytwo.ripple.extensions.test;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class BagElements extends PrimitiveFunction
{
    public BagElements( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "bagElements" ), mc );
    }

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        Resource head;

        head = mc.castToResource( stack.getFirst() );
        stack = stack.getRest();

        Iterator<Value> iter = mc.bagValue( head ).iterator();
        while ( iter.hasNext() )
            sink.put( new ListNode<Value>( iter.next(), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
