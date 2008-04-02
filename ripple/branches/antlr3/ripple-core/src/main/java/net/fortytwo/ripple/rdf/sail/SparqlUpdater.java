/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.rdf.diff.RdfDiffContextFilter;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.diff.RdfDiffSource;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.UriMap;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.NameValuePair;
import org.apache.commons.httpclient.methods.PostMethod;
import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;

/**
 * Note: this class is not thread-safe.
 * @author josh
 */
public class SparqlUpdater
{
	private RdfDiffContextFilter contextFilter;
	private RdfDiffSink sink;
	private UriMap uriMap;

	public SparqlUpdater( final UriMap uriMap, final RdfDiffSink sink )
	{
		this.uriMap = uriMap;
		this.sink = sink;

		contextFilter = new RdfDiffContextFilter();
	}

	public RdfDiffSink getSink()
	{
		return contextFilter;
	}

	public void flush() throws RippleException
	{
		Iterator<Resource> contexts = contextFilter.contextIterator();
		while ( contexts.hasNext() )
		{
			Resource context = contexts.next();
			RdfDiffSource source = contextFilter.sourceForContext( context );

			// Some statements cannot be written to the Semantic Web.
			if ( null != context
					&& context instanceof URI
					&& RdfUtils.isHttpUri( (URI) context ) )
			{
				String url = uriMap.get( context.toString() );

				try
				{
					postUpdate( url, source );
				}

				catch ( RippleException e )
				{
					e.logError();
				}
			}

// The statements written to the triple store should depend on the outcome of
// the update operation (if any).
source.writeTo( sink );
		}

		contextFilter.clear();
	}

	private void postUpdate( final String url, final RdfDiffSource source )
		throws RippleException
	{
		String postData = createPostData( source );
System.out.println( "posting update to url <" + url + ">: " + postData );

		PostMethod method = HttpUtils.createSparqlUpdateMethod( url.toString() );
        NameValuePair[] data = {   // FIXME: is this correct?
                new NameValuePair( HttpUtils.BODY, postData )
              };
		method.setRequestBody(data);
		HttpUtils.registerMethod( method );

		HttpClient client = HttpUtils.createClient();
		
		int responseCode;

		try
		{
			client.executeMethod( method );
			
			// ...do something with the response..
			
			responseCode = method.getStatusCode();
			method.releaseConnection();
		}
		
		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

System.out.println( "response code = " + responseCode );
	}

	private static String createPostData( final RdfDiffSource source ) throws RippleException
	{
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		PrintStream ps = new PrintStream( bos );

		SesameOutputAdapter adapter
			= RdfUtils.createOutputAdapter( bos, RDFFormat.TURTLE );

		ps.println( "INSERT {" );
		adapter.startRDF();
		source.adderSource().namespaceSource().writeTo( adapter.namespaceSink() );
		source.adderSource().statementSource().writeTo( adapter.statementSink() );
		adapter.endRDF();
		ps.println( "}" );

		// Note: since some statements are rejected, we will sometimes end up
		// with an empty DELETE graph.
		ps.println( "DELETE {" );
		adapter.startRDF();
// TODO: ignore statements with blank nodes as subject or object... UNLESS they're found to serve some purpose
		source.subtractorSource().namespaceSource().writeTo( adapter.namespaceSink() );
		source.subtractorSource().statementSource().writeTo( adapter.statementSink() );
		adapter.endRDF();
		ps.println( "}" );

		return bos.toString();
	}
}

// kate: tab-width 4
