package wurfel.model.combinators;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.Apply;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_L extends Combinator
{
    private static final URI
        s_uri = Wurfel.getWurfelTestUri( "L" );

    public Combinator_L( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      Context context )
        throws WurfelException
    {
        Value x, y;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();
        y = argIter.next();

        Value result = new Apply(
            x,
            new Apply( y, y ) );

        return new NodeSet( result );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
