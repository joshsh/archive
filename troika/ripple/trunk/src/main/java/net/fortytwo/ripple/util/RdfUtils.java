package net.fortytwo.ripple.util;

import java.io.InputStream;
import java.io.OutputStream;

import java.net.URL;
import java.net.URLConnection;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;

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

	/**
	 *  @param urlConn  an already-connected URLConnection
	 */
	public static RDFFormat read( final URLConnection urlConn,
								final RdfSourceAdapter adapter,
								final String baseUri,
								final RDFFormat format )
		throws RippleException
	{
		InputStream response;

		try
		{
			response = urlConn.getInputStream();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		read( response, adapter, baseUri, format );

		try
		{
			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		return format;
	}

	/**
	 *  @param urlConn  an already-connected URLConnection
	 */
	public static RDFFormat read( final URLConnection urlConn,
								final RdfSourceAdapter adapter,
								final String baseUri )
		throws RippleException
	{
		InputStream response;

		try
		{
			response = urlConn.getInputStream();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		RDFFormat format = HttpUtils.guessRdfFormat( urlConn );
		if ( null == format )
			// Soft fail (possibly too soft?)
			return null;

		read( response, adapter, baseUri, format );

		try
		{
			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		return format;
	}

	public static RDFFormat read( final URL url,
								final RdfSourceAdapter adapter,
								final String baseUri,
								RDFFormat format )
		throws RippleException
	{
		URLConnection urlConn = HttpUtils.openConnection( url );
		HttpUtils.prepareUrlConnectionForRdfRequest( urlConn );
		HttpUtils.connect( urlConn );

		return read( urlConn, adapter, baseUri, format );
	}

	public static RDFFormat read( final URL url,
								final RdfSourceAdapter adapter,
								final String baseUri )
		throws RippleException
	{
		URLConnection urlConn = HttpUtils.openConnection( url );
		HttpUtils.prepareUrlConnectionForRdfRequest( urlConn );
		HttpUtils.connect( urlConn );

		return read( urlConn, adapter, baseUri );
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
		if ( s.equals( "n3" ) )
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
}

// kate: tab-width 4
