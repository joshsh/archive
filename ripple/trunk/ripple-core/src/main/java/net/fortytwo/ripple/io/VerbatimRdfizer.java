package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import org.openrdf.model.URI;
import org.openrdf.rio.*;

import java.io.IOException;
import java.io.InputStream;

/**
 * Author: josh
 * Date: Jan 16, 2008
 * Time: 12:15:21 PM
 */
public class VerbatimRdfizer implements Rdfizer
{
	private RDFParser parser;

	public VerbatimRdfizer( final RDFFormat format )
	{
		parser = Rio.createParser( format );
	}

	public Outcome handle( final InputStream is,
						  final RDFHandler handler,
						  final URI documentUri,
						  final String baseUri )
	{
		try
		{
			parser.setRDFHandler( handler );
			parser.parse( is, baseUri );
		}

		catch ( IOException e )
		{
			new RippleException( e ).logError();
			return Outcome.Failure;
		}

		catch ( RDFParseException e )
		{
			new RippleException( e ).logError();
			return Outcome.Failure;
		}

		catch ( RDFHandlerException e ) {
			new RippleException( e ).logError();
			return Outcome.Failure;
		}

		return Outcome.Success;
	}
}
