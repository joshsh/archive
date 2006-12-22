package wurfel.primitives;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class ListElements extends PrimitiveFunction
{
    private static final URI s_uri
        = Wurfel.getWurfelTestUri( "listElements" );

    public ListElements( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        Resource head;

        Iterator<Value> argIter = args.iterator();
        head = context.castToResource( argIter.next() );

        return new NodeSet( context.listValue( head ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on