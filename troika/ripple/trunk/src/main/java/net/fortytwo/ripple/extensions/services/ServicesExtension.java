package net.fortytwo.ripple.extensions.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class ServicesExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/services#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "services.ttl" ) + "#" );

		bridge.add( new PingTheSemanticWeb( new RdfValue( mc.createUri( ns + "pingTheSemanticWeb" ) ), mc ), mc );
		bridge.add( new Sindice( new RdfValue( mc.createUri( ns + "sindice" ) ), mc ), mc );
		bridge.add( new SwoogleIt( new RdfValue( mc.createUri( ns + "swoogleIt" ) ), mc ), mc );
	}
}

// kate: tab-width 4
