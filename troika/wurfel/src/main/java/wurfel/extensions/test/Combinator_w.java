package wurfel.extensions.test;

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

public class Combinator_w extends Combinator
{
    public Combinator_w( Context context )
        throws WurfelException
    {
        super( Wurfel.getWurfelTestUri( "w" ), context );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      Context context )
        throws WurfelException
    {
        Value x;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();

        Value result = new Apply(
            x,
            x );

        return new NodeSet( result );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
