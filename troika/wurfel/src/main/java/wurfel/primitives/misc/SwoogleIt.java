package wurfel.primitives.misc;

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

public class SwoogleIt extends PrimitiveFunction
{
    private static final URI
        s_uri = Wurfel.getWurfelTestUri( "swoogleIt" ),
        s_rdfTypeUri = Wurfel.getRdfUri( "type" ),
        s_swoogleQueryResponseUri = Wurfel.getSwoogleUri( "QueryResponse" );

    public SwoogleIt( Context context )
        throws WurfelException
    {
        super( s_uri, context );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               Context context )
        throws WurfelException
    {
        String key, searchString;
        URI queryType;

        Iterator<Value> argIter = args.iterator();
        key = context.stringValue(
                context.castToLiteral( argIter.next() ) );
        queryType = context.castToUri( argIter.next() );
        searchString = context.stringValue(
                context.castToLiteral( argIter.next() ) );

        try
        {
            URL url = new URL(
                "http://logos.cs.umbc.edu:8080/swoogle31/q"
                + "?key=" + URLEncoder.encode( key )
                + "&queryType=" + URLEncoder.encode( queryType.getLocalName() )
                + "&searchString=" + URLEncoder.encode( searchString ) );

            URI baseUri = Wurfel.createRandomUri();
            context.importModel( url, baseUri );
            NodeSet results = new NodeSet();
//System.out.println( "baseUri = " + baseUri );

            Repository repository = context.getRepository();
                Connection conn = repository.getConnection();
                boolean includeInferred = true;
                CloseableIterator<? extends Statement> stmtIter
                    = conn.getStatements(
//                        null, s_rdfTypeUri, s_swoogleQueryResponseUri, includeInferred );
                        null, s_rdfTypeUri, s_swoogleQueryResponseUri, baseUri, includeInferred );
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
