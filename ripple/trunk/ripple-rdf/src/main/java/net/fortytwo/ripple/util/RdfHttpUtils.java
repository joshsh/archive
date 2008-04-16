/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-utils/src/main/java/net/fortytwo/ripple/rdf/RdfUtils.java $
 * $Revision: 1042 $
 * $Author: josh $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.rdf.RdfUtils;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.openrdf.rio.RDFFormat;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.JarURLConnection;
import java.net.URL;

public final class RdfHttpUtils
{
	public static RDFFormat read( final HttpMethod method,
								final SesameInputAdapter sa,
								final String baseUri,
								RDFFormat format ) throws RippleException
	{
		HttpUtils.registerMethod( method );

		InputStream body;

		HttpClient client = HttpUtils.createClient();

		try
		{
			client.executeMethod( method );
	        body = method.getResponseBodyAsStream();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

        if ( null == format )
        {
        	format = RdfUtils.guessRdfFormat( method.getPath(),
        			method.getResponseHeader( HttpUtils.CONTENT_TYPE ).getValue() );

        	if ( null == format )
        	{
        		// TODO: logger message?
//System.out.println("coudn't guess format");
        		return null;
        	}
        }

		RdfUtils.read( body, sa, baseUri, format );

		try
		{
			body.close();
		}

		catch ( IOException e )
		{
			throw new RippleException( e );
		}

        method.releaseConnection();

		return format;
	}

	public static RDFFormat read( final URL url,
								final SesameInputAdapter sa,
								final String baseUri,
								RDFFormat format ) throws RippleException
	{
		String urlStr = url.toString();

		if ( urlStr.startsWith( "jar:" ) )
		{
			if ( null == format )
			{
				format = RdfUtils.guessRdfFormat( urlStr, null );
			}

			JarURLConnection jc;
			InputStream is;

			try
			{
				jc = (JarURLConnection) url.openConnection();
				is = jc.getInputStream();
			}

			catch ( IOException e )
			{
				throw new RippleException( e );
			}

			RdfUtils.read( is, sa, baseUri, format );

			try
			{
				is.close();

				// Note: apparently it's not necessary to disconnect from a
				// JarURLConnection.
			}

			catch ( IOException e )
			{
				throw new RippleException( e );
			}

			return format;
		}

		else if ( urlStr.startsWith( "file:" ) )
		{
			if ( null == format )
			{
				format = RdfUtils.guessRdfFormat( urlStr, null );
			}

			InputStream is;

			try
			{
				is = new FileInputStream( urlStr.substring( 5 ) );
			}

			catch ( IOException e )
			{
				throw new RippleException( e );
			}

			RdfUtils.read( is, sa, baseUri, format );

			try
			{
				is.close();
			}

			catch ( IOException e )
			{
				throw new RippleException( e );
			}

			return format;
		}

		else
		{
			HttpMethod method = HttpUtils.createRdfGetMethod( url.toString() );
			return read( method, sa, baseUri, format );
		}
		/*
		URLConnection uc = HttpUtils.openConnection( url );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );

		return readPrivate( uc, sa, baseUri, format );
		*/
	}

	public static RDFFormat read( final URL url,
								final SesameInputAdapter sa,
								final String baseUri )
		throws RippleException
	{
		return read( url, sa, baseUri, null );
	}
}
