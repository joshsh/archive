package wurfel.extensions.misc;

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

public class UriToString extends PrimitiveFunction
{
    public UriToString( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelMiscUri( "uriToString" ), evalContext );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        URI a;
        String result;

        Iterator<Value> argIter = args.iterator();
        a = evalContext.castToUri( argIter.next() );

        try
        {
            result = a.toString();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( evalContext.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
