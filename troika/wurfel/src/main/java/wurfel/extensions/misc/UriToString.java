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

public class UriToString extends PrimitiveFunction
{
    public UriToString( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "uriToString" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        URI a;
        String result;

        Iterator<Value> argIter = args.iterator();
        a = mc.castToUri( argIter.next() );

        try
        {
            result = a.toString();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( mc.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
