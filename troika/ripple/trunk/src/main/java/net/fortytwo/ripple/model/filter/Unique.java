package net.fortytwo.ripple.model.filter;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ListMemoizer;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Unique implements Function
{
    private static String memo = "memo";
    private ListMemoizer<Value,String> memoizer = null;

    public Unique()
    {
//System.out.println( "" + this + "()" );
    }

    ////////////////////////////////////////////////////////////////////////////

public URI getUri()
{
return null;
}

    public int arity()
    {
        return 1;
    }

public void checkArguments( ListNode<Value> args )
    throws RippleException
{}

public void printTo( RipplePrintStream p )
    throws RippleException
{}

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

    ////////////////////////////////////////////////////////////////////////////

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        if ( null == memoizer )
        {
            memoizer = new ListMemoizer<Value,String>( stack, memo );
            sink.put( stack );
//System.out.println( "put first: " + stack.getFirst() );
        }

        else if ( memoizer.add( stack, memo ) )
//{
            sink.put( stack );
//System.out.println( "put another: " + stack.getFirst() );
//}
//else
//{
//System.out.println( "rejected this: " + stack.getFirst() );
//}
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
