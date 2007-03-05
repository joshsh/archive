package net.fortytwo.ripple.extensions.newstuff;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.Combinator;
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

public class IntegerLessThan extends PrimitiveFunction
{
    public IntegerLessThan( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "integer-lessThan" ), mc );
    }

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        int a, b;
        Combinator result;

        a = mc.intValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();
        b = mc.intValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

// TODO: we shouldn't need to create a new Combinator instance for each result.
        result = ( a < b )
            ? new True( mc )
            : new False( mc );

        sink.put( new ListNode<Value>( result, stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
