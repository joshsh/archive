/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class ServicesExtension extends Extension
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/services#";

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			NS, getClass().getResource( "services.ttl" ) + "#" );

		registerPrimitive( PingTheSemanticWeb.class, NS + "pingTheSemanticWeb", mc );
		registerPrimitive( Sindice.class, NS + "sindice", mc );
		registerPrimitive( SwoogleIt.class, NS + "swoogleIt", mc );
		registerPrimitive( Uriqr.class, NS + "uriqr", mc );
	}
}

// kate: tab-width 4
