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

public class IntegerMultiply extends PrimitiveFunction
{
    public IntegerMultiply( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "integer-multiply" ), mc );
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
        result = a * b;

        return new NodeSet( mc.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
