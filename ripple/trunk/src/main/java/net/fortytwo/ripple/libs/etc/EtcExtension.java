/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class EtcExtension extends Extension
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/etc#";

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			NS, getClass().getResource( "etc.ttl" ) + "#" );

		registerPrimitive( Get.class, NS + "get", mc );
		registerPrimitive( Time.class, NS + "time", mc );
	}
}

// kate: tab-width 4
