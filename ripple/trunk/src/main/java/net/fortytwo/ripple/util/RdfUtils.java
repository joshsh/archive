/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.io.InputStream;
import java.io.OutputStream;

import java.net.URL;
import java.net.URLConnection;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;

import org.apache.log4j.Logger;

import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParser;
import org.openrdf.rio.RDFWriter;
import org.openrdf.sail.memory.MemoryStoreRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;

public class RdfUtils
{
	final static Logger logger = Logger.getLogger( RdfUtils.class );

	public static Repository createMemoryStoreRepository()
		throws RippleException
	{
		try
		{
			Repository repository = Ripple.useInference()
				? new SailRepository(
					new MemoryStoreRDFSInferencer(
						new MemoryStore() ) )
				: new SailRepository(
					new MemoryStore() );

			repository.initialize();

			return repository;
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	public static RDFFormat read( final InputStream is,
								final RdfSourceAdapter adapter,
								final String baseUri,
								final RDFFormat format )
		throws RippleException
	{
		try
		{
			RDFParser parser = Rio.createParser( format /*, valueFactory */ );
			parser.setRDFHandler( adapter );
			parser.parse( is, baseUri );
		}

		catch ( Throwable t )
		{
			// Soft fail.
			new RippleException( t );
		}

		return format;
	}

	static RDFFormat readPrivate( final URLConnection uc,
									final RdfSourceAdapter adapter,
									final String baseUri,
									final RDFFormat format )
		throws RippleException
	{
		final Pointer<RDFFormat> formatPtr = new Pointer<RDFFormat>();
		formatPtr.ref = format;

		// Don't wait indefinitely for a connection.
		// Note: this timeout applies only to the establishment of a connection.
		//       A document may take arbitrarily long to retrieve and parse.
		uc.setConnectTimeout( (int) Ripple.urlConnectTimeout() );
		HttpUtils.connect( uc );

// 		new ThreadWrapper( "for RDFFormat readPrivate" )
// 		{
// 			protected void run() throws RippleException
// 			{
				if ( null == formatPtr.ref )
					// This operation may hang as well.
					formatPtr.ref = guessRdfFormat( uc );
// 			}
// 		}.start(  );

		if ( null == formatPtr.ref )
			// Soft fail (possibly too soft?)
			return null;

		InputStream response;

		try
		{
			response = uc.getInputStream();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		read( response, adapter, baseUri, formatPtr.ref );

		try
		{
			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		return formatPtr.ref;
	}

	/**
	 *  @param uc  an already-connected URLConnection
	 */
	public static RDFFormat read( final URLConnection uc,
								final RdfSourceAdapter adapter,
								final String baseUri,
								final RDFFormat format )
		throws RippleException
	{
		return readPrivate( uc, adapter, baseUri, format );
	}

	/**
	 *  @param uc  an already-connected URLConnection
	 */
	public static RDFFormat read( final URLConnection uc,
								final RdfSourceAdapter adapter,
								final String baseUri )
		throws RippleException
	{
		return readPrivate( uc, adapter, baseUri, null );
	}

	public static RDFFormat read( final URL url,
								final RdfSourceAdapter adapter,
								final String baseUri,
								RDFFormat format )
		throws RippleException
	{
		URLConnection uc = HttpUtils.openConnection( url );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );

		return readPrivate( uc, adapter, baseUri, format );
	}

	public static RDFFormat read( final URL url,
								final RdfSourceAdapter adapter,
								final String baseUri )
		throws RippleException
	{
		return read( url, adapter, baseUri, null );
	}

	// Note: not thread-safe with respect to the repository.
	public static void write( final Repository repo,
								final OutputStream out,
								final RDFFormat format )
		throws RippleException
	{
		try
		{
			RepositoryConnection rc = repo.getConnection();

			// Note: a comment by Jeen suggests that a new writer should be created
			//       for each use:
			//       http://www.openrdf.org/forum/mvnforum/viewthread?thread=785#3159
			RDFWriter writer = Rio.createWriter( format, out );
			rc.export( writer );

			rc.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	public static RDFFormat findFormat( final String name )
	{
		RDFFormat format;

		String s = name.toLowerCase();

		// Try not to be picky.
		if ( s.equals( "n3" ) || s.equals( "notation3" ) )
			format = RDFFormat.N3;
		else if ( s.equals( "ntriples" ) || s.equals( "n-triples" ) )
			format = RDFFormat.NTRIPLES;
		else if ( s.equals( "rdfxml" ) || s.equals( "rdf/xml" ) )
			format = RDFFormat.RDFXML;
		else if ( s.equals( "trig" ) )
			format = RDFFormat.TRIG;
		else if ( s.equals( "trix" ) )
			format = RDFFormat.TRIX;
		else if ( s.equals( "turtle" ) )
			format = RDFFormat.TURTLE;

		// Alright, be picky.
		else
			// Note: this may be null.
			format = RDFFormat.valueOf( s );

		return format;
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
		logger.debug( "contentType = " + contentType );

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
		logger.debug( "extension = " + ext );

		// Primary content type rules.
		if ( null != contentType )
		{
			// See: http://www.w3.org/TR/rdf-syntax-grammar/
			if ( contentType.contains( "application/rdf+xml" ) )
			{
				return RDFFormat.RDFXML;
			}

			// See: http://www.w3.org/DesignIssues/Notation3.html
			else if ( contentType.contains( "text/rdf+n3" ) )
			{
				return RDFFormat.N3;
			}

// See: RDFFormat.TRIX.getMIMEType()
			else if ( contentType.contains( "application/trix" ) )
			{
				return RDFFormat.TRIX;
			}

			else if ( contentType.contains( "application/x-trig" ) )
			{
				return RDFFormat.TRIG;
			}

			// See: http://www.dajobe.org/2004/01/turtle/
			else if ( contentType.contains( "application/x-turtle" ) )
			{
				return RDFFormat.TURTLE;
			}

			// This will generate a lot of false positives, but we might as well
			// *try* to parse a text document.
			else if ( contentType.contains( "text/plain" ) )
			{
				return RDFFormat.NTRIPLES;
			}
		}

		// Primary file extension rules.
		if ( null != ext )
		{
			if ( ext.equals( "n3" ) )
			{
				return RDFFormat.N3;
			}

			else if ( ext.equals( "nt" ) )
			{
				return RDFFormat.NTRIPLES;
			}

			else if ( ext.equals( "rdf" )
					|| ext.equals( "rdfs" )
					|| ext.equals( "owl" )

					// examples:
					//     http://www.aaronsw.com/about.xrdf
					//     http://www.w3.org/People/karl/karl-foaf.xrdf
					|| ext.equals( "xrdf" ) )
			{
				return RDFFormat.RDFXML;
			}

// Note: another common extension for TriX files is ".xml"
			else if ( ext.equals( "trix" ) )
			{
				return RDFFormat.TRIX;
			}

			else if ( ext.equals( "trig" ) )
			{
				return RDFFormat.TRIG;
			}

			else if ( ext.equals( "ttl" ) )
			{
				return RDFFormat.TURTLE;
			}
		}

		// Secondary content type rules.
		if ( null != contentType )
		{
			if ( contentType.contains( "application/xml" ) )
			{
				return RDFFormat.RDFXML;
			}

			// precedent: http://www.mindswap.org/2004/owl/mindswappers
			else if ( contentType.contains( "text/xml" ) )
			{
				return RDFFormat.RDFXML;
			}

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
			{
				return RDFFormat.RDFXML;
			}
		}

		// Blacklisting rules.  There are some common content types which are
		// not worth trying.
		if ( null != contentType )
		{
			if ( contentType.contains( "text/html" ) )
			{
				return null;
			}
		}

		// Last-ditch rule.
		return RDFFormat.RDFXML;
	}
}

// kate: tab-width 4