package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class IntegerDivide extends PrimitiveFunction
{
    public IntegerDivide( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "integer-divide" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        int a, b, result;

        Iterator<Value> argIter = args.iterator();
        a = mc.intValue(
                mc.castToLiteral( argIter.next() ) );
        b = mc.intValue(
                mc.castToLiteral( argIter.next() ) );

        try
        {
            result = a / b;
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( mc.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
