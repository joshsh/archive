package wurfel.model.primitives;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class IntegerAdd extends PrimitiveFunction
{
    private static final URI s_uri
        = Wurfel.getWurfelTestUri( "integer-add" );

    public IntegerAdd( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        int a, b, result;

        Iterator<Value> argIter = args.iterator();
        a = context.intValue(
                context.castToLiteral( argIter.next() ) );
        b = context.intValue(
                context.castToLiteral( argIter.next() ) );
        result = a + b;

        return new NodeSet( context.toLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
