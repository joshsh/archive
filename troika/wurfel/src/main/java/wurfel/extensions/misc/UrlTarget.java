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

import java.net.URL;
import java.net.URLConnection;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class UrlTarget extends PrimitiveFunction
{
    public UrlTarget( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelMiscUri( "urlTarget" ), evalContext );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        String a, result;

        Iterator<Value> argIter = args.iterator();
        a = evalContext.stringValue(
                evalContext.castToLiteral( argIter.next() ) );

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

            response.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return new NodeSet( evalContext.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
