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

    public FunctionEnvelope( URI uri )
    {
        func = new ForwardPredicateQuery( uri );
    }

    public FunctionEnvelope( Function function )
    {
        func = function;
    }

    public FunctionEnvelope( ListNode<Value> list )
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

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
