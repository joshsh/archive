package net.fortytwo.ripple.util;

import java.io.InputStream;

import java.net.URLConnection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;

import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParser;

public class RdfUtils
{
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
