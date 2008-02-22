/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/io/HttpUriDereferencer.java $
 * $Revision: 961 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.StringUtils;
import org.apache.log4j.Logger;
import org.restlet.resource.Representation;

import java.util.HashSet;
import java.util.Set;

// Note: throughout this implementation, both the caching context of a URI and
//       its associated web location are the same as its success or failure 'memo'.
public class HttpUriDereferencer implements UriDereferencer
{
	private static final Logger LOGGER = Logger.getLogger( HttpUriDereferencer.class );

	// FIXME: temporary
	private WebClosure webClosure;

	private Set<String> badExtensions;

	public HttpUriDereferencer( final WebClosure webClosure )
	{
		this.webClosure = webClosure;

		badExtensions = new HashSet<String>();

	}

	public Representation dereference( final String uri ) throws RippleException
	{
	    // Don't dereference a URI which appears to point to a file which is not
		// an RDF document.
		int l = uri.lastIndexOf( '.' );
		if ( l >= 0 && badExtensions.contains( uri.substring( l + 1 ) ) )
		{
			throw new RippleException( "URI <" + StringUtils.escapeUriString( uri ) + "> has blacklisted extension" );
			// TODO: we can throw exceptions or return nulls to indicate an error, but we shouldn't do both
//			return null;
		}

        return new HttpRepresentation( uri, webClosure.getAcceptHeader() );
	}

	public void blackListExtension( final String ext )
	{
		badExtensions.add( ext );
	}

	public void whitelistExtension( final String ext )
	{
		badExtensions.remove( ext );
	}
}

// kate: tab-width 4
