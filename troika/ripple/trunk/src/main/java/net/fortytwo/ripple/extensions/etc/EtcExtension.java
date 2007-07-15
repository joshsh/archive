package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class EtcExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/etc#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "etc.ttl" ) + "#" );

		registerPrimitive( Back.class, ns + "back", mc );
		registerPrimitive( Get.class, ns + "get", mc );
		registerPrimitive( Pred.class, ns + "pred", mc );
		registerPrimitive( Prim.class, ns + "prim", mc );
		registerPrimitive( Rand.class, ns + "rand", mc );
		registerPrimitive( Time.class, ns + "time", mc );

		// Note: the xml: namespace is actually said to be
		//       http://www.w3.org/XML/1998/namespace
		//       (i.e. without the hash character).
		registerPrimitive( Lang.class, "http://www.w3.org/XML/1998/namespace#lang", mc );

		// Note: this URI is bogus.
		registerPrimitive( Type.class, "http://www.w3.org/2001/XMLSchema#type", mc );
	}
}

// kate: tab-width 4
