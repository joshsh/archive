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
		registerPrimitive( Pred.class, NS + "pred", mc );
		registerPrimitive( Prim.class, NS + "prim", mc );
		registerPrimitive( Time.class, NS + "time", mc );

		// Note: the xml: namespace is actually said to be
		//       http://www.w3.org/XML/1998/namespace
		//       (i.e. without the hash character).
		registerPrimitive( Lang.class, "http://www.w3.org/XML/1998/namespace#lang", mc );

		// Note: this URI is bogus.
		registerPrimitive( Type.class, "http://www.w3.org/2001/XMLSchema#type", mc );
	}
}

// kate: tab-width 4
