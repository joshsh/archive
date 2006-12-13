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

public class Combinator_V extends Combinator
{
    private static final URI
        s_uri = Wurfel.getWurfelTestUri( "V" );

    public Combinator_V( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      Context context )
        throws WurfelException
    {
        Value x, y, z;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();
        y = argIter.next();
        z = argIter.next();

        Value result = new Apply(
            new Apply( z, x ),
            y );

        return new NodeSet( result );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
