package wurfel.extensions.test;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.Apply;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_V extends Combinator
{
    public Combinator_V( EvaluationContext evalContext )
        throws WurfelException
    {
        super( Wurfel.getWurfelTestUri( "V" ), evalContext );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      EvaluationContext evalContext )
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