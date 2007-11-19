/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.io.PrintStream;

import java.net.HttpURLConnection;
import java.net.URL;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.rdf.diff.RdfDiffContextFilter;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.diff.RdfDiffSource;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.util.UrlFactory;

import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;

public class SparqlUpdater
{
	private RdfDiffContextFilter contextFilter;
	private RdfDiffSink sink;
	private UrlFactory urlFactory;

	public SparqlUpdater( final UrlFactory urlFactory, final RdfDiffSink sink )
	{
		this.urlFactory = urlFactory;
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
				URL url = urlFactory.createUrl( context.toString() );

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

	private void postUpdate( final URL url, final RdfDiffSource source )
		throws RippleException
	{
		String postData = createPostData( source );
System.out.println( "posting update to url <" + url + ">: " + postData );

try
{
		HttpURLConnection huc = (HttpURLConnection) url.openConnection();
		HttpUtils.prepareUrlConnectionForSparqlUpdate( huc );
		OutputStream os = huc.getOutputStream();
		PrintStream ps = new PrintStream( os );
		ps.print( postData );
		ps.close();
		os.close();

		int responseCode = huc.getResponseCode();
System.out.println( "response code = " + responseCode );
}

catch ( Throwable t )
{
	throw new RippleException( t );
}
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
