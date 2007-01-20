package wurfel.extensions.test;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
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

public class ListElements extends PrimitiveFunction
{
    public ListElements( Context context )
        throws WurfelException
    {
        super( Wurfel.getWurfelTestUri( "listElements" ), context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        Resource head;
        Context context = evalContext.getContext();

        Iterator<Value> argIter = args.iterator();
        head = context.castToResource( argIter.next() );

        return new NodeSet( context.listValue( head ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
