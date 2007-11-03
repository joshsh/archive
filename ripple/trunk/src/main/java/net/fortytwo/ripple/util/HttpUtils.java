/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

public class HttpUtils
{
	private static Map<String, Date> lastRequestByHost = new HashMap<String, Date>();

	public static void prepareUrlConnectionForTextRequest( final URLConnection urlConn )
	{
		setAgent( urlConn );

		StringBuilder sb = new StringBuilder();
		sb.append( "text/plain" );
		sb.append( ", text/xml" );
		sb.append( ", text/html" );
		sb.append( ", text/plain" );
		sb.append( ", *; q=0.2" );
		sb.append( ", */*; q=0.2" );
		urlConn.setRequestProperty( "Accept", sb.toString() );
	}

	public static void prepareUrlConnectionForRequest(
		final URLConnection urlConn,
		final String [] mimeTypes )
	{
		setAgent( urlConn );

		StringBuilder sb = new StringBuilder();
		for ( int i = 0; i < mimeTypes.length; i++ )
		{
			if ( i > 0 )
			{
				sb.append( ", " );
			}

			sb.append( mimeTypes[i] );
		}
		urlConn.setRequestProperty( "Accept", sb.toString() );
	}

	public static void prepareUrlConnectionForRdfRequest( final URLConnection urlConn )
	{
		setAgent( urlConn );

		setRdfAcceptHeader( urlConn );

// To consider at some point: caching, authorization
	}

	public static void prepareUrlConnectionForSparqlUpdate( final HttpURLConnection urlConn )
		throws RippleException
	{
		setAgent( urlConn );

		urlConn.setDoOutput( true );

		try
		{
			urlConn.setRequestMethod( "POST" );
		}

		catch ( java.net.ProtocolException e )
		{
			throw new RippleException( e );
		}

		urlConn.setRequestProperty( "Content-type", "application/sparql-query" );
		setRdfAcceptHeader( urlConn );
	}

	private static void setRdfAcceptHeader( final URLConnection urlConn )
	{
		/* Comment by arjohn in http://www.openrdf.org/forum/mvnforum/viewthread?thread=805#3234
			Note that Sesame/Rio doesn't have a real N3 parser, but it does have a Turtle parser, which supports a much larger subset of N3. At first sight, I would say that the Turtle parser should be able to parse the data fragment that you posted. */
		boolean n3DeserializationSupported = false;

		StringBuilder sb = new StringBuilder();
		sb.append( "application/rdf+xml" );
		if ( n3DeserializationSupported )
		{
			sb.append( ", text/rdf+n3" );
		}
		sb.append( ", application/trix" );
		sb.append( ", application/x-turtle" );
		sb.append( ", text/plain" );
		sb.append( ", application/xml;q=0.5" );
		sb.append( ", text/xml;q=0.2" );
		urlConn.setRequestProperty( "Accept", sb.toString() );
	}

	/**
	 *  A wrapper for URLConnection.connect() which enforces crawler etiquette.
	 *  That is, it avoids the Ripple client making a nuisance of itself by
	 *  making too many requests, too quickly, of the same host.
     *  TODO: request and respect robots.txt, if present.
     */
	public static void connect( final URLConnection urlConn )
		throws RippleException
	{
//LOGGER.info( "connecting to: " + urlConn.getURL() );
		String host = urlConn.getURL().getHost();
//LOGGER.info( "    host = " + host );

		// Some connections (e.g. file system operations) have no host.  Don't
		// bother regulating them.
		if ( null != host && host.length() > 0 )
		{
			Date now = new Date();
			long delay = Ripple.urlConnectCourtesyInterval();
//LOGGER.info( "    delay = " + delay );
	
			Date lastRequest;
			long w = 0;

			synchronized ( lastRequestByHost )
			{
				lastRequest = lastRequestByHost.get( host );

				// We've already made a request of this host.
				if ( null != lastRequest )
				{
					// If it hasn't been long enough since the last request from the same
					// host, wait a bit before issuing a new request.
					if ( now.getTime() - lastRequest.getTime() < delay )
					{
						w = lastRequest.getTime() + delay - now.getTime();
					}
				}

				// Record the projected start time of the request beforehand, to
				// avoid any other requests being scheduled without knowledge of
				// this one.
				lastRequestByHost.put( host, new Date( w + now.getTime() ) );
			}
	
			// Wait if necessary.
			if ( w > 0 )
			{
				try
				{
//LOGGER.info( "    waiting " + w + " milliseconds" );
					Thread.sleep( w );
				}

				catch ( InterruptedException e )
				{
					throw new RippleException( e );
				}
			}
		}

		try
		{
			// Open a communcations link with the host.
			urlConn.connect();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

// 		catch ( java.net.SocketTimeoutException e )
// 		{
// 			throw new RippleException( e );
// 		}
	}

	public static URLConnection openConnection( final URL url )
		throws RippleException
	{
		try
		{
			return url.openConnection();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}

	public static URLConnection openConnection( final String urlStr )
		throws RippleException
	{
		URL url;

		try
		{
			url = new URL( urlStr );
		}

		catch ( java.net.MalformedURLException e )
		{
			throw new RippleException( e );
		}

		return openConnection( url );
	}

	public InputStream getInputStream( final URLConnection uc )
		throws RippleException
	{
		try
		{
			return uc.getInputStream();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}

	public static void showUrlConnection( final URLConnection urlConn )
	{
		Map<String, List<String>> requestProperties
			= urlConn.getRequestProperties();
		Set<String> keys = requestProperties.keySet();

		StringBuilder sb = new StringBuilder();
		sb.append( "Request properties:\n" );

		Iterator<String> keyIter = keys.iterator();
		while ( keyIter.hasNext() )
		{
			String key = keyIter.next();
			sb.append( "\t" + key + ": " );
			Iterator<String> valueIter = requestProperties.get( key ).iterator();

			boolean first = true;
			while ( valueIter.hasNext() )
			{
				String value = valueIter.next();
				if ( first )
				{
					first = false;
				}
				else
				{
					sb.append( ", " );
				}
				sb.append( value );
			}

			sb.append( "\n" );
		}

		System.out.println( sb.toString() );
	}

	////////////////////////////////////////////////////////////////////////////

	private static void setAgent( final URLConnection urlConn )
	{
		urlConn.setRequestProperty( "User-Agent",
			Ripple.getName() + "/" + Ripple.getVersion() );
	}
}

// kate: tab-width 4
