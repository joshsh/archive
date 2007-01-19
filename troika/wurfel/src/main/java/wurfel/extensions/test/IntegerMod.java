package wurfel.extensions.test;

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

public class IntegerMod extends PrimitiveFunction
{
    public IntegerMod( Context context )
        throws WurfelException
    {
        super( Wurfel.getWurfelTestUri( "integer-mod" ), context );
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

        try
        {
            result = a % b;
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( context.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
