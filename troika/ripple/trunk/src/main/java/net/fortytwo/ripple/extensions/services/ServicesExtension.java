package net.fortytwo.ripple.extensions.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class ServicesExtension extends Extension
{
	static String ns = "http://fortytwo.net/2007/05/ripple/services#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "services.ttl" ) + "#" );

		registerPrimitive( PingTheSemanticWeb.class, ns + "pingTheSemanticWeb", mc );
		registerPrimitive( Sindice.class, ns + "sindice", mc );
		registerPrimitive( SwoogleIt.class, ns + "swoogleIt", mc );
		registerPrimitive( Uriqr.class, ns + "uriqr", mc );
	}
}

// kate: tab-width 4
