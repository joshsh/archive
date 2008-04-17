/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.URIMap;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Library;

/**
 * A collection of hooks into selected web services.
 */
public class ServicesLibrary extends Library
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/services#";

	public void load( final URIMap uf, final ModelConnection mc )
		throws RippleException
	{
		uf.put(
			NS, getClass().getResource( "services.ttl" ) + "#" );

		registerPrimitive( PingTheSemanticWeb.class, NS + "pingTheSemanticWeb", mc );
		registerPrimitive( SwoogleIt.class, NS + "swoogleIt", mc );
		registerPrimitive( Uriqr.class, NS + "uriqr", mc );
	}
}

// kate: tab-width 4
