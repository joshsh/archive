package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;

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
        throws WurfelException
    {
        selfUri = self;
        arityCached = mc.intValue(
            mc.castToLiteral(
                mc.findUniqueProduct(
                    selfUri, mc.createWurfelUri( "combinatorArity" ) ) ) );
    }

    public int arity()
    {
        return arityCached;
    }

    public void checkArguments( LinkedList<Value> args )
        throws WurfelException
    {
        if ( args.size() != arity() )
            throw new WurfelException( "attempt to apply a "
                + arity() + "-ary function to a list of "
                + args.size() + " arguments" );
    }

    public String toString()
    {
        return selfUri.toString();
    }

    public void printTo( WurfelPrintStream p )
        throws WurfelException
    {
        p.print( selfUri );
    }

    public Value toRdf( ModelConnection mc )
        throws WurfelException
    {
        return selfUri;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
