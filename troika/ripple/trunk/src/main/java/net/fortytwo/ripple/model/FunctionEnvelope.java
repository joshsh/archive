package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ListMemoizer;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class FunctionEnvelope implements RippleValue
{
    Function func;

    private FunctionEnvelope( URI uri )
    {
        func = new ForwardPredicateQuery( uri );
    }

    private FunctionEnvelope( Function function )
    {
        func = function;
    }

    private FunctionEnvelope( ListNode<Value> list )
    {
        func = new ListDequotation( list );
    }

    public Function getFunction()
    {
        return func;
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        p.print( "/" );
        p.print( func );
    }

    public static FunctionEnvelope createEnvelope( Value v )
        throws RippleException
    {
        if ( v instanceof URI )
            return new FunctionEnvelope( (URI) v );
        else if ( v instanceof Function )
            return new FunctionEnvelope( (Function) v );
        else if ( v instanceof ListNode )
            return new FunctionEnvelope( (ListNode<Value>) v );
        else
            throw new RippleException( "bad Value in FunctionEnvelope createEnvelope()" );
    }

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
