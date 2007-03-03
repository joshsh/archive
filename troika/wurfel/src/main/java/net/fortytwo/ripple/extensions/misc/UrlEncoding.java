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

import java.net.URLEncoder;

public class UrlEncoding extends PrimitiveFunction
{
    public UrlEncoding( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "urlEncoding" ), mc );
    }

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        String a, result;

        a = mc.stringValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

        try
        {
            result = URLEncoder.encode( a );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        sink.put( new ListNode<Value>( mc.createLiteral( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
