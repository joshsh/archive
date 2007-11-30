package net.fortytwo.rdfwiki;

import org.openrdf.sail.Sail;
import org.restlet.data.Request;

public interface SailSelector
{
	Sail selectSail( Request request ) throws Exception;
}
