package net.fortytwo.ripple.extensions.misc;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class UriToString extends PrimitiveFunction
{
    public UriToString( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "uriToString" ), mc );
    }

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        URI a;
        String result;

        a = mc.castToUri( stack.getFirst() );
        stack = stack.getRest();

        try
        {
            result = a.toString();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        sink.put( new ListNode<Value>( mc.createLiteral( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
