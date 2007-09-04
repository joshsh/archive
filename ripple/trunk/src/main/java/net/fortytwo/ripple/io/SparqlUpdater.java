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

import java.net.URL;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Statement;
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFWriter;

public class SparqlUpdater
{
	public static void postUpdate( final RdfDiff diff, final URL url )
		throws RippleException
	{
		String postData = createPostData( diff );

System.out.println( "would have posted this data: " + postData );


/*
		HttpUrlConnection huc = url.openConnection();
		huc.setRequestMethod( "POST" );
		huc.setDoOutput( true );
		OutputStream os = huc.getOutputStream();
		
		....
		
		os.close();
		int responseCode = huc.getResponseCode();
System.out.println( "reponse code = " + responseCode );
*/

	}

	static String createPostData( final RdfDiff diff ) throws RippleException
	{
		Iterator<Statement> addIter = diff.getAddedIterator();
		Iterator<Statement> subIter = diff.getSubtractedIterator();

		if ( !addIter.hasNext() && !subIter.hasNext() )
			return "";

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

		if ( addIter.hasNext() )
		{
			ps.println( "INSERT {" );

			while ( addIter.hasNext() )
			{
				Statement st = addIter.next();

				try
				{
					writer.startRDF();
					writer.handleStatement( st );
					writer.endRDF();
				}

				catch ( Throwable t )
				{
					throw new RippleException( t );
				}
			}

			ps.println( "}" );
		}

		// Note: since some statements are rejected, we will sometimes end up
		// with an empty DELETE graph.
		if ( subIter.hasNext() )
		{
			ps.println( "DELETE {" );

				Statement st = addIter.next();

// TODO: ignore statements with blank nodes as subject or object... UNLESS they're found to serve some purpose
				try
				{
					writer.startRDF();
					writer.handleStatement( st );
					writer.endRDF();
				}

				catch ( Throwable t )
				{
					throw new RippleException( t );
				}

			ps.println( "}" );
		}

		return bos.toString();
	}
}

// kate: tab-width 4
