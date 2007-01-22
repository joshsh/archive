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
    private URI
        rdfTypeUri,
        swoogleQueryResponseUri;

    public SwoogleIt( EvaluationContext evalContext )
        throws WurfelException
    {
        super( Wurfel.createUri( "urn:net.dnsdojo.troika.wurfel-misc#swoogleIt" ), evalContext );

        rdfTypeUri = Wurfel.getRdfUri( "type" );
        swoogleQueryResponseUri = Wurfel.getSwoogleUri( "QueryResponse" );
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
            URL url = new URL(
                "http://logos.cs.umbc.edu:8080/swoogle31/q"
                + "?key=" + URLEncoder.encode( key )
                + "&queryType=" + URLEncoder.encode( queryType.getLocalName() )
                + "&searchString=" + URLEncoder.encode( searchString ) );

            URI baseUri = Wurfel.createRandomUri();
            context.importModel( url, baseUri );
            NodeSet results = new NodeSet();
//System.out.println( "baseUri = " + baseUri );

            Connection conn = evalContext.getConnection();
            boolean includeInferred = true;
            CloseableIterator<? extends Statement> stmtIter
                = conn.getStatements(
//                    null, rdfTypeUri, swoogleQueryResponseUri, includeInferred );
                    null, rdfTypeUri, swoogleQueryResponseUri, baseUri, includeInferred );
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
