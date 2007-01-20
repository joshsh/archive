package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URLEncoder;

public class UrlEncoding extends PrimitiveFunction
{
    public UrlEncoding( Context context )
        throws WurfelException
    {
        super( Wurfel.createUri( "urn:net.dnsdojo.troika.wurfel-misc#urlEncoding" ), context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        String a, result;
        Context context = evalContext.getContext();

        Iterator<Value> argIter = args.iterator();
        a = context.stringValue(
                context.castToLiteral( argIter.next() ) );

        try
        {
            result = URLEncoder.encode( a );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( context.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
