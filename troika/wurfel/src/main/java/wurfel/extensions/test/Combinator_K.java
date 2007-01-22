package wurfel.extensions.test;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_K extends Combinator
{
    public Combinator_K( EvaluationContext evalContext )
        throws WurfelException
    {
        super( Wurfel.getWurfelTestUri( "K" ), evalContext );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      EvaluationContext evalContext )
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