package net.fortytwo.ripple.util;

import java.net.URLConnection;

import net.fortytwo.ripple.Ripple;

public class HttpUtils
{
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

	private static void setAgent( final URLConnection urlConn )
	{
		urlConn.setRequestProperty( "User-Agent",
			Ripple.getName() + "/" + Ripple.getVersion() );
	}
}

// kate: tab-width 4
