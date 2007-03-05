package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.LinkedList;

public abstract class Combinator extends Node implements Function
{
    protected int arityCached;

    protected URI selfUri;
    public URI getUri()
    {
        return selfUri;
    }

    protected Combinator( final URI self, ModelConnection mc )
        throws RippleException
    {
        selfUri = self;
        arityCached = mc.intValue(
            mc.castToLiteral(
                mc.findUniqueProduct(
                    selfUri, mc.createRippleUri( "combinatorArity" ) ) ) );
    }

    public int arity()
    {
        return arityCached;
    }

    public void checkArguments( ListNode<Value> args )
        throws RippleException
    {
/*
        if ( args.size() != arity() )
            throw new RippleException( "attempt to apply a "
                + arity() + "-ary function to a list of "
                + args.size() + " arguments" );
*/
    }

    public String toString()
    {
        return selfUri.toString();
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        p.print( selfUri );
    }

    public Value toRdf( ModelConnection mc )
        throws RippleException
    {
        return selfUri;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
