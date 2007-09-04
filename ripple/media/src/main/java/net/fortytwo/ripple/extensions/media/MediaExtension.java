/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.media;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class MediaExtension extends Extension
{
	static String ns = "http://fortytwo.net/2007/08/ripple/media#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "media.ttl" ) + "#" );

		registerPrimitive( Play.class, ns + "play", mc );
		registerPrimitive( Show.class, ns + "show", mc );
		registerPrimitive( Speak.class, ns + "speak", mc );
	}
}

// kate: tab-width 4
