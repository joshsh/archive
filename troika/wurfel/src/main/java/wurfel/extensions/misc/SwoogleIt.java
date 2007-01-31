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

    public SwoogleIt( EvaluationContext evalContext )
        throws WurfelException
    {
        super( evalContext.createWurfelMiscUri( "swoogleIt" ), evalContext );

        rdfTypeUri = evalContext.createRdfUri( "type" );
        swoogleQueryResponseUri = evalContext.createSwoogleUri( "QueryResponse" );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        String key, searchString;
        URI queryType;
        Context context = evalContext.getContext();

        Iterator<Value> argIter = args.iterator();
        key = evalContext.stringValue(
                evalContext.castToLiteral( argIter.next() ) );
        queryType = evalContext.castToUri( argIter.next() );
        searchString = evalContext.stringValue(
                evalContext.castToLiteral( argIter.next() ) );

        try
        {
            String urlStr = "http://logos.cs.umbc.edu:8080/swoogle31/q"
                + "?key=" + URLEncoder.encode( key )
                + "&queryType=" + URLEncoder.encode( queryType.getLocalName() )
                + "&searchString=" + URLEncoder.encode( searchString );

            URL url = new URL( urlStr );

            URI baseUri = evalContext.createUri( urlStr );
            evalContext.addGraph( url, baseUri );
            NodeSet results = new NodeSet();
//System.out.println( "baseUri = " + baseUri );

            CloseableIterator<? extends Statement> stmtIter
                = evalContext.getConnection().getStatements(
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
