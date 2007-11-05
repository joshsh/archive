/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.media;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.Library;
import net.fortytwo.ripple.util.UrlFactory;

public class MediaLibrary extends Library
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/media#";

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			NS, getClass().getResource( "media.ttl" ) + "#" );

		registerPrimitive( Play.class, NS + "play", mc );
		registerPrimitive( Show.class, NS + "show", mc );
		registerPrimitive( Speak.class, NS + "speak", mc );
	}
}

// kate: tab-width 4
