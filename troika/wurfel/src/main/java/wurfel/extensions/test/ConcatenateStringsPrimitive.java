package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.Container;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class ConcatenateStringsPrimitive extends PrimitiveFunction
{
    public ConcatenateStringsPrimitive( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "concatenateStrings" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        String strA, strB, result;

        Iterator<Value> argIter = args.iterator();
        strA = mc.stringValue(
                    mc.castToLiteral( argIter.next() ) );
        strB = mc.stringValue(
                    mc.castToLiteral( argIter.next() ) );
        result = strA + strB;

        return new Container( mc.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
