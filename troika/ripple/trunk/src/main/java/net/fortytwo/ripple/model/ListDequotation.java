package net.fortytwo.ripple.model;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class ListDequotation implements Function
{
    ListNode<Value> reversedList;

    private static ListNode<Value> reverse( final ListNode<Value> list )
    {
        ListNode<Value> in = list;
        ListNode<Value> out = null;
        while ( null != in )
        {
            out = new ListNode<Value>( in.getFirst(), out );
            in = in.getRest();
        }

        return out;
    }

    public ListDequotation( ListNode<Value> list )
    {
        reversedList = reverse( list );
    }

    public int arity()
    {
return 1;
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        ListNode<Value> in = reversedList;
        ListNode<Value> out = stack;

        while ( null != in )
        {
            out = new ListNode<Value>( in.getFirst(), out );
            in = in.getRest();
        }

        sink.put( out );
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
p.print( reversedList );
    }

public URI getUri()
{
return null;
}

public void checkArguments( ListNode<Value> args )
    throws RippleException
{}

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
