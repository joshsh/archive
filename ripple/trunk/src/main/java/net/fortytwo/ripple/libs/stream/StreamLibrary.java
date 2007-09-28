/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Library;
import net.fortytwo.ripple.util.UrlFactory;

public class StreamLibrary extends Library
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/stream#";

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			NS, getClass().getResource( "stream.ttl" ) + "#" );

		registerPrimitive( Each.class, NS + "each", mc );
		registerPrimitive( Limit.class, NS + "limit", mc );
		registerPrimitive( Scrap.class, NS + "scrap", mc );
		registerPrimitive( Union.class, NS + "union", mc );
		registerPrimitive( Unique.class, NS + "unique", mc );
	}
}

// kate: tab-width 4
