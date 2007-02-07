package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class BagElements extends PrimitiveFunction
{
    public BagElements( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelTestUri( "bagElements" ), evalContext );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        Resource head;

        Iterator<Value> argIter = args.iterator();
        head = evalContext.castToResource( argIter.next() );

        return evalContext.bagValue( head );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
