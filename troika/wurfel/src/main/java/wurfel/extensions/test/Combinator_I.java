package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.Apply;
import wurfel.model.Combinator;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_I extends Combinator
{
    public Combinator_I( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelTestUri( "I" ), evalContext );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      EvaluationContext evalContext )
        throws WurfelException
    {
        Value x;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();

        return new NodeSet( x );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
