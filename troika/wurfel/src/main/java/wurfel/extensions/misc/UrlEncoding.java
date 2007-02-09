package wurfel.extensions.misc;

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

import java.net.URLEncoder;

public class UrlEncoding extends PrimitiveFunction
{
    public UrlEncoding( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "urlEncoding" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        String a, result;

        Iterator<Value> argIter = args.iterator();
        a = mc.stringValue(
                mc.castToLiteral( argIter.next() ) );

        try
        {
            result = URLEncoder.encode( a );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( mc.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
