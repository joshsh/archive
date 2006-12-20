package wurfel.combinators;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_K extends Combinator
{
    private static final URI
        s_uri = Wurfel.getWurfelTestUri( "K" );

    public Combinator_K( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      Context context )
        throws WurfelException
    {
        Value x;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();
//        Value y = argIter.next();

        return new NodeSet( x );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
