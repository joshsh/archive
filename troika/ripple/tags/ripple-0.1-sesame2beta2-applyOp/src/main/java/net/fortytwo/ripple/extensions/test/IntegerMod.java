package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
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

public class IntegerMod extends PrimitiveFunction
{
    public IntegerMod( ModelConnection mc )
        throws RippleException
    {
        super( mc.createRippleTestUri( "integer-mod" ), mc );
    }

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        int a, b, result;

        a = mc.intValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();
        b = mc.intValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

        try
        {
            result = a % b;
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        sink.put( new ListNode<Value>( mc.createLiteral( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
