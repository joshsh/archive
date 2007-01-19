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

public class ConcatenateStringsPrimitive extends PrimitiveFunction
{
    public ConcatenateStringsPrimitive( Context context )
        throws WurfelException
    {
        super( Wurfel.getWurfelTestUri( "concatenateStrings" ), context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        String strA, strB, result;

        Iterator<Value> argIter = args.iterator();
        strA = context.stringValue(
                    context.castToLiteral( argIter.next() ) );
        strB = context.stringValue(
                    context.castToLiteral( argIter.next() ) );
        result = strA + strB;

        return new NodeSet( context.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
