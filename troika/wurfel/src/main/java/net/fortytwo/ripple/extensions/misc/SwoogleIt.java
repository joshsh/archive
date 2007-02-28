package net.fortytwo.ripple.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.repository.RepositoryResult;

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

    protected void applyInternal( ListNode<Value> stack,
                                  Sink<ListNode<Value>> sink,
                                  ModelConnection mc )
        throws WurfelException
    {
        String key, searchString;
        URI queryType;

        key = mc.stringValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();
        queryType = mc.castToUri( stack.getFirst() );
        stack = stack.getRest();
        searchString = mc.stringValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

        try
        {
            String urlStr = "http://logos.cs.umbc.edu:8080/swoogle31/q"
                + "?key=" + URLEncoder.encode( key )
                + "&queryType=" + URLEncoder.encode( queryType.getLocalName() )
                + "&searchString=" + URLEncoder.encode( searchString );

            URL url = new URL( urlStr );

            URI baseUri = mc.createUri( urlStr );
            mc.addGraph( url, baseUri );
//System.out.println( "baseUri = " + baseUri );

            RepositoryResult<Statement> stmtIter
                = mc.getRepositoryConnection().getStatements(
//                    null, rdfTypeUri, swoogleQueryResponseUri, includeInferred );
                    null, rdfTypeUri, swoogleQueryResponseUri, /*baseUri,*/ Wurfel.useInference() );
            while ( stmtIter.hasNext() )
                sink.put( stack.push( stmtIter.next().getSubject() ) );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
