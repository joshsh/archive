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
	ContextMemo.Status handle( InputStream is, RDFHandler handler, URI resourceUri, String baseUri );
}
