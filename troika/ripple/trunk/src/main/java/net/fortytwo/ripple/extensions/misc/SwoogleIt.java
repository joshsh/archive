package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

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

	private static String swoogleNs = "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#";
	
    public SwoogleIt( RdfValue v, ModelConnection mc )
        throws RippleException
    {
        super( v, mc );

        rdfTypeUri = mc.createRdfUri( "type" );
        swoogleQueryResponseUri = mc.createUri( swoogleNs + "QueryResponse" );
    }

    public void applyTo( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        String key, searchString;
        URI queryType;

        key = mc.stringValue( stack.getFirst() );
        stack = stack.getRest();
        queryType = mc.uriValue( stack.getFirst() );
        stack = stack.getRest();
        searchString = mc.stringValue( stack.getFirst() );
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
                    null, rdfTypeUri, swoogleQueryResponseUri, /*baseUri,*/ Ripple.useInference() );
            while ( stmtIter.hasNext() )
                sink.put( new RippleStack( new RdfValue( stmtIter.next().getSubject() ), stack ) );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
