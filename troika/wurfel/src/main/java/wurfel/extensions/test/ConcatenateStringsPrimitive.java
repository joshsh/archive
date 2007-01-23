package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class ConcatenateStringsPrimitive extends PrimitiveFunction
{
    public ConcatenateStringsPrimitive( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelTestUri( "concatenateStrings" ), evalContext );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        String strA, strB, result;

        Iterator<Value> argIter = args.iterator();
        strA = evalContext.stringValue(
                    evalContext.castToLiteral( argIter.next() ) );
        strB = evalContext.stringValue(
                    evalContext.castToLiteral( argIter.next() ) );
        result = strA + strB;

        return new NodeSet( evalContext.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
