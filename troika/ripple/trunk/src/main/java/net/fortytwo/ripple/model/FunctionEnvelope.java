package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.util.Sink;

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

    private FunctionEnvelope( RippleStack list )
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
        else if ( v instanceof RippleStack )
            return new FunctionEnvelope( (RippleStack) v );
        else
            throw new RippleException( "bad Value in FunctionEnvelope createEnvelope()" );
    }

    public boolean isFunctionEnvelope()
    {
        return true;
    }

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
