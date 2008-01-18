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
	Representation handle( String uri ) throws RippleException;
}

// kate: tab-width 4
