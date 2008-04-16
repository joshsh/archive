/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.rdfwiki;

import org.openrdf.sail.Sail;
import org.restlet.data.Request;

public interface SailSelector
{
	Sail selectSail( Request request ) throws Exception;
}
