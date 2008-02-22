/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import org.restlet.resource.Representation;

public interface UriDereferencer
{
	/**
	 *
	 * @param uri
	 * @return
	 * @throws RippleException
	 */
	// TODO: this method throws an exception, while Rdfizer.handle does not
	Representation dereference( String uri ) throws RippleException;
}

// kate: tab-width 4
