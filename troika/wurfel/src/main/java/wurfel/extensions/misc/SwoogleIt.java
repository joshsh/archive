package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.util.iterator.CloseableIterator;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URL;
import java.net.URLEncoder;

public class SwoogleIt extends PrimitiveFunction
{
    private URI
        rdfTypeUri,
        swoogleQueryResponseUri;

    public SwoogleIt( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelMiscUri( "swoogleIt" ), mc );

        rdfTypeUri = mc.createRdfUri( "type" );
        swoogleQueryResponseUri = mc.createSwoogleUri( "QueryResponse" );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        String key, searchString;
        URI queryType;

        Iterator<Value> argIter = args.iterator();
        key = mc.stringValue(
                mc.castToLiteral( argIter.next() ) );
        queryType = mc.castToUri( argIter.next() );
        searchString = mc.stringValue(
                mc.castToLiteral( argIter.next() ) );

        try
        {
            String urlStr = "http://logos.cs.umbc.edu:8080/swoogle31/q"
                + "?key=" + URLEncoder.encode( key )
                + "&queryType=" + URLEncoder.encode( queryType.getLocalName() )
                + "&searchString=" + URLEncoder.encode( searchString );

            URL url = new URL( urlStr );

            URI baseUri = mc.createUri( urlStr );
            mc.addGraph( url, baseUri );
            NodeSet results = new NodeSet();
//System.out.println( "baseUri = " + baseUri );

            CloseableIterator<? extends Statement> stmtIter
                = mc.getConnection().getStatements(
//                    null, rdfTypeUri, swoogleQueryResponseUri, includeInferred );
                    null, rdfTypeUri, swoogleQueryResponseUri, baseUri, Wurfel.useInference() );
            while ( stmtIter.hasNext() )
                results.add( stmtIter.next().getSubject() );
            stmtIter.close();

            return results;
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
