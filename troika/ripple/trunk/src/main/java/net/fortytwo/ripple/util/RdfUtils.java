package net.fortytwo.ripple.util;

import java.io.InputStream;

import java.net.URLConnection;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;

import org.openrdf.repository.Repository;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParser;
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
//                    new MemoryStore( new java.io.File( "net.fortytwo.ripple.tmp" ) ) ) );

			repository.initialize();

			return repository;
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	public static void read( final URLConnection urlConn,
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
		RDFParser parser = Rio.createParser( format /*, valueFactory */ );
		parser.setRDFHandler( adapter );

		try
		{
			parser.parse( response, urlConn.getURL().toString() );
		}

		catch ( Throwable t )
		{
			// Soft fail.
			new RippleException( t );
		}

		try
		{
			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
