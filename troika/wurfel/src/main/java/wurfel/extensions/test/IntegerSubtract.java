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

public class IntegerSubtract extends PrimitiveFunction
{
    public IntegerSubtract( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelTestUri( "integer-subtract" ), evalContext );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        int a, b, result;

        Iterator<Value> argIter = args.iterator();
        a = evalContext.intValue(
                evalContext.castToLiteral( argIter.next() ) );
        b = evalContext.intValue(
                evalContext.castToLiteral( argIter.next() ) );
        result = a - b;

        return new NodeSet( evalContext.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
