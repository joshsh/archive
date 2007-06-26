package net.fortytwo.ripple.util;

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

import org.apache.log4j.Logger;

import org.openrdf.rio.RDFFormat;

public class HttpUtils
{
	final static Logger s_logger
		= Logger.getLogger( HttpUtils.class );

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
				sb.append( ", " );
			sb.append( mimeTypes[i] );
		}
		urlConn.setRequestProperty( "Accept", sb.toString() );
	}

	public static void prepareUrlConnectionForRdfRequest( final URLConnection urlConn )
	{
		setAgent( urlConn );

		/* Comment by arjohn in http://www.openrdf.org/forum/mvnforum/viewthread?thread=805#3234
			Note that Sesame/Rio doesn't have a real N3 parser, but it does have a Turtle parser, which supports a much larger subset of N3. At first sight, I would say that the Turtle parser should be able to parse the data fragment that you posted. */
		boolean n3DeserializationSupported = false;

		StringBuilder sb = new StringBuilder();
		sb.append( "application/rdf+xml" );
		if ( n3DeserializationSupported )
			sb.append( ", text/rdf+n3" );
		sb.append( ", application/trix" );
		sb.append( ", application/x-turtle" );
		sb.append( ", text/plain" );
		sb.append( ", application/xml;q=0.5" );
		sb.append( ", text/xml;q=0.2" );
		urlConn.setRequestProperty( "Accept", sb.toString() );

// To consider at some point: caching, authorization
	}

	// Note: examines the content type first, then the URL extension.  If all
	//       else fails, default to RDF/XML and hope for the best.
	public static RDFFormat guessRdfFormat( final URLConnection urlConn )
	{
/*
System.out.println( RDFFormat.N3.getName() + ": " + RDFFormat.N3.getMIMEType() );
System.out.println( RDFFormat.NTRIPLES.getName() + ": " + RDFFormat.NTRIPLES.getMIMEType() );
System.out.println( RDFFormat.RDFXML.getName() + ": " + RDFFormat.RDFXML.getMIMEType() );
System.out.println( RDFFormat.TRIX.getName() + ": " + RDFFormat.TRIX.getMIMEType() );
System.out.println( RDFFormat.TURTLE.getName() + ": " + RDFFormat.TURTLE.getMIMEType() );
*/
		String contentType = urlConn.getContentType();
		s_logger.debug( "contentType = " + contentType );

		String file = urlConn.getURL().getFile();
		String ext;
		if ( null == file )
			ext = null;
		else
		{
			int lastDot = file.lastIndexOf( '.' );
			ext = ( lastDot > 0 && lastDot < file.length() - 1 )
				? file.substring( lastDot + 1 )
				: null;
		}
		s_logger.debug( "extension = " + ext );

		// Primary content type rules.
		if ( null != contentType )
		{
			// See: http://www.w3.org/TR/rdf-syntax-grammar/
			if ( contentType.contains( "application/rdf+xml" ) )
				return RDFFormat.RDFXML;

			// See: http://www.w3.org/DesignIssues/Notation3.html
			else if ( contentType.contains( "text/rdf+n3" ) )
				return RDFFormat.N3;

// See: RDFFormat.TRIX.getMIMEType()
			else if ( contentType.contains( "application/trix" ) )
				return RDFFormat.TRIX;

			// See: http://www.dajobe.org/2004/01/turtle/
			else if ( contentType.contains( "application/x-turtle" ) )
				return RDFFormat.TURTLE;
		}

		// Primary file extension rules.
		if ( null != ext )
		{
			if ( ext.equals( "n3" ) )
				return RDFFormat.N3;

			else if ( ext.equals( "nt" ) )
				return RDFFormat.NTRIPLES;

			else if ( ext.equals( "rdf" )
			|| ext.equals( "rdfs" )
			|| ext.equals( "owl" ) )
				return RDFFormat.RDFXML;

// TODO: is this actually a TriX file extension?
			else if ( ext.equals( "trix" ) )
				return RDFFormat.TRIX;

			else if ( ext.equals( "ttl" )
			|| ext.equals( "turtle" ) )
				return RDFFormat.TURTLE;

// example:
//     http://www.aaronsw.com/about.xrdf
//     http://www.w3.org/People/karl/karl-foaf.xrdf
			else if ( ext.equals( "xrdf" ) )
				return RDFFormat.RDFXML;
		}

		// Secondary content type rules.
		if ( null != contentType )
		{
			if ( contentType.contains( "application/xml" ) )
				return RDFFormat.RDFXML;

			// precedent: http://www.mindswap.org/2004/owl/mindswappers
			else if ( contentType.contains( "text/xml" ) )
				return RDFFormat.RDFXML;

			// See: http://www.w3.org/TR/rdf-testcases/#ntriples)
			// This is only a secondary rule because the text/plain MIME type
			// is so broad, and the N-Triples format so uncommon.
//            else if ( contentType.contains( "text/plain" ) )
//                return RDFFormat.NTRIPLES;
		}

		// Secondary file extension rules.
		if ( null != ext )
		{
			// precedent:
			//     http://hometown.aol.com/chbussler/foaf/chbussler.foaf
			if ( ext.equals( "foaf" ) )
				return RDFFormat.RDFXML;
		}

		// Blacklisting rules.  There are some common content types which are
		// not worth trying.
		if ( null != contentType )
		{
			if ( contentType.contains( "text/html" ) )
				return null;
		}

		// Last-ditch rule.
		return RDFFormat.RDFXML;
	}

	static Map<String, Date> lastRequestByHost = new HashMap<String, Date>();

	/**
	 *  A wrapper for URLConnection.connect() which enforces crawler etiquette.
	 *  That is, it avoids the Ripple client making a nuisance of itself by
	 *  making too many requests, too quickly, of the same host.
     *  TODO: request and respect robots.txt, if present.
     */
	public static void connect( final URLConnection urlConn )
		throws RippleException
	{
s_logger.info( "connecting to: " + urlConn.getURL() );
		String host = urlConn.getURL().getHost();
s_logger.info( "    host = " + host );
		// Some connections (e.g. file system operations) have no host.  Don't
		// bother regulating them.
		if ( null != host && host.length() > 0 )
		{
			Date now = new Date();
			long delay = Ripple.getCourtesyDelay();
s_logger.info( "    delay = " + delay );
	
			Date lastRequest = lastRequestByHost.get( host );
	
			// We've already made a request of this host.
			if ( null != lastRequest )
			{
				// If it hasn't been long enough since the last request from the same
				// host, wait a bit before issuing a new request.
				long millisSinceLastRequest = now.getTime() - lastRequest.getTime();
				if ( millisSinceLastRequest < delay )
				{
					// Avoid any other requests of the same host getting through
					// while this one is waiting.
					lastRequestByHost.put( host, now );
	
					long w = lastRequest.getTime() + delay - now.getTime();
s_logger.info( "    waiting " + w + " milliseconds" );

					try
					{
						synchronized( now )
						{
							now.wait( w );
						}
					}
	
					catch ( InterruptedException e )
					{
						throw new RippleException( e );
					}
	
					// Update current time.
					now = new Date();
				}
			}
	
			// Record this event.
			lastRequestByHost.put( host, now );
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

	public static void showUrlConnection( final URLConnection urlConn )
	{
		Map<String,List<String>> requestProperties
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
					first = false;
				else
					sb.append( ", " );
				sb.append( value );
			}

			sb.append( "\n" );
		}

		System.out.println( sb.toString() );
	}

	////////////////////////////////////////////////////////////////////////////

	static void setAgent( final URLConnection urlConn )
	{
		urlConn.setRequestProperty( "User-Agent",
			Ripple.getName() + "/" + Ripple.getVersion() );
	}
}

// kate: tab-width 4
