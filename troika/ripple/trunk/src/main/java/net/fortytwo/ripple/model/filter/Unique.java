package net.fortytwo.ripple.model.filter;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ListMemoizer;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Unique extends Filter
{
    private static String memo = "memo";
    private ListMemoizer<Value,String> memoizer = null;

    public Unique()
    {
//System.out.println( "" + this + "()" );
    }

    public void applyTo( RippleStack stack,
                         Sink<RippleStack> sink,
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
