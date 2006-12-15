package wurfel.model.primitives.misc;

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

import java.net.URL;
import java.net.URLConnection;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class UrlTarget extends PrimitiveFunction
{
    private static final URI s_uri
        = Wurfel.getWurfelTestUri( "urlTarget" );

    public UrlTarget( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        String a, result;

        Iterator<Value> argIter = args.iterator();
        a = context.stringValue(
                context.castToLiteral( argIter.next() ) );

        try
        {
            URL url = new URL( a );
            URLConnection connection = url.openConnection();
            connection.connect();

            InputStream response = connection.getInputStream();

            BufferedReader br = new BufferedReader(
                new InputStreamReader( response ) );
            StringBuffer sb = new StringBuffer();
            String nextLine = "";
            while ( ( nextLine = br.readLine() ) != null )
                 sb.append(nextLine);
            result = sb.toString();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( context.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
