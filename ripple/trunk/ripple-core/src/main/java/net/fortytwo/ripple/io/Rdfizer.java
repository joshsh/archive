package net.fortytwo.ripple.io;

import org.openrdf.model.URI;
import org.openrdf.rio.RDFHandler;

import java.io.InputStream;

/**
 * Author: josh
 * Date: Jan 16, 2008
 * Time: 12:07:22 PM
 */
public interface Rdfizer
{
	enum Outcome {
			Undetermined,       // to be used only when a memo is created
			Success,            // normal outcome
			Timeout,            // network timeout
			InvalidUri,         // bad URI
			ParseError,         // a document was received, but failed to parse
			ClientError,        // 4xx HTTP error
			ServerError,        // 5xx HTTP error
			BadUriScheme,       // no suitable URI dereferencer was found
			BadMediaType,       // no suitable rdfizer was found
			DereferencerError,  // TODO: break this down into more specific conditions
			RdfizerError,       // TODO: break this down into more specific conditions
			Ignore,             // don't bother dereferencing these URIs
			Failure             // all other error conditions
	}

	Outcome handle( InputStream is, RDFHandler handler, URI documentUri, String baseUri );
}
