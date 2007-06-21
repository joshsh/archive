package net.fortytwo.ripple.extensions.media;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class MediaExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/media#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "media.ttl" ) + "#" );

		bridge.add( new Show( new RdfValue( mc.createUri( ns + "show" ) ), mc ), mc );
		bridge.add( new Speak( new RdfValue( mc.createUri( ns + "speak" ) ), mc ), mc );
	}
}

// kate: tab-width 4
