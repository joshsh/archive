package net.fortytwo.ripple.extensions.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class StreamExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/08/ripple/stream#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "stream.ttl" ) + "#" );

		registerPrimitive( Each.class, ns + "each", mc );
		registerPrimitive( Limit.class, ns + "limit", mc );
		registerPrimitive( Scrap.class, ns + "scrap", mc );
		registerPrimitive( Union.class, ns + "union", mc );
		registerPrimitive( Unique.class, ns + "unique", mc );
	}
}

// kate: tab-width 4
