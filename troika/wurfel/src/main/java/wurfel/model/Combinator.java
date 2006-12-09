package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.LinkedList;

public abstract class Combinator extends Node implements Function
{
    private static final URI
        s_wurfelCombinatorArityUri = Wurfel.getWurfelUri( "combinatorArity" );

    protected int arityCached;

    protected URI selfUri;
    public URI getUri()
    {
        return selfUri;
    }

    public Combinator( final URI self, Context context )
        throws WurfelException
    {
        selfUri = self;
        arityCached = context.getInteger( selfUri, s_wurfelCombinatorArityUri );
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
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
