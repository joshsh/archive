package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.repository.Connection;
import org.openrdf.repository.Repository;
import org.openrdf.util.iterator.CloseableIterator;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URL;
import java.net.URLEncoder;

public class Grab extends PrimitiveFunction
{
    private static final URI
        s_uri = Wurfel.createUri( "urn:net.dnsdojo.troika.wurfel-misc#grab" );

    public Grab( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        String urlStr;

        Iterator<Value> argIter = args.iterator();
        urlStr = context.stringValue(
                context.castToLiteral( argIter.next() ) );

        try
        {
            URL url = new URL( urlStr );

            URI baseUri = Wurfel.createRandomUri();
            context.importModel( url, baseUri );
            NodeSet results = new NodeSet();

            Repository repository = context.getRepository();
            Connection conn = repository.getConnection();
            boolean includeInferred = true;
            CloseableIterator<? extends Statement> stmtIter
                = conn.getStatements(
                    null, null, null, baseUri, includeInferred );
            while ( stmtIter.hasNext() )
                results.add( stmtIter.next().getSubject() );
            stmtIter.close();
            conn.close();

            return results;
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
