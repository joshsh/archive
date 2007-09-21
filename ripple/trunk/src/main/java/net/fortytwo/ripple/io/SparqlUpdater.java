/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.OutputStream;
import java.io.InputStream;

import java.net.URL;
import java.net.HttpURLConnection;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.HttpUtils;

import org.openrdf.model.Statement;
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFWriter;

public final class SparqlUpdater
{
	private SparqlUpdater()
	{
	}

	public static void postUpdate( final RdfDiff diff, final URL url )
		throws RippleException
	{
		String postData = createPostData( diff );

System.out.println( "about to post this data: " + postData );

try
{
		HttpURLConnection huc = (HttpURLConnection) url.openConnection();
		HttpUtils.prepareUrlConnectionForSparqlUpdate( huc );
		OutputStream os = huc.getOutputStream();
		
// 		SesameOutputAdapter soa = new SesameOutputAdapter(
// 			new RDFXMLWriter( os ) );
// 		soa.startRDF();
// 		...
// 		soa.endRDF();

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

	private static String createPostData( final RdfDiff diff ) throws RippleException
	{
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		PrintStream ps = new PrintStream( bos );

		RDFWriter writer;

		try
		{
			writer = Rio.createWriter( RDFFormat.TURTLE, bos );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		SesameOutputAdapter adapter = new SesameOutputAdapter( writer );

// 		if ( diff.countAdded() > 0 )
// 		{
			ps.println( "INSERT {" );
			adapter.startRDF();

			diff.adderSource().statementSource().writeTo( adapter.statementSink() );

			adapter.endRDF();
			ps.println( "}" );
// 		}

		// Note: since some statements are rejected, we will sometimes end up
		// with an empty DELETE graph.
// 		if ( diff.countSubtracted() > 0 )
// 		{
			ps.println( "DELETE {" );
			adapter.startRDF();

// TODO: ignore statements with blank nodes as subject or object... UNLESS they're found to serve some purpose
			diff.subtractorSource().statementSource().writeTo( adapter.statementSink() );

			adapter.endRDF();
			ps.println( "}" );
// 		}

		return bos.toString();
	}
}

// kate: tab-width 4
