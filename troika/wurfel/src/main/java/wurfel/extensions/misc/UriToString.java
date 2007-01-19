package wurfel.extensions.misc;

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

public class UriToString extends PrimitiveFunction
{
    public UriToString( Context context )
        throws WurfelException
    {
        super( Wurfel.createUri( "urn:net.dnsdojo.troika.wurfel-misc#uriToString" ), context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        URI a;
        String result;

        Iterator<Value> argIter = args.iterator();
        a = context.castToUri( argIter.next() );

        try
        {
            result = a.toString();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( context.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
