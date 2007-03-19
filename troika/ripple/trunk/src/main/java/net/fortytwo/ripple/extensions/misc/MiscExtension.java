package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class MiscExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/rpl-misc#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new Sha1SumOf( new RdfValue( mc.createUri( ns + "sha1SumOf" ) ), mc ), mc );
		bridge.add( new UriToString( new RdfValue( mc.createUri( ns + "uriToString" ) ), mc ), mc );
		bridge.add( new UrlEncoding( new RdfValue( mc.createUri( ns + "urlEncoding" ) ), mc ), mc );
		bridge.add( new UrlTarget( new RdfValue( mc.createUri( ns + "urlTarget" ) ), mc ), mc );
		bridge.add( new SwoogleIt( new RdfValue( mc.createUri( ns + "swoogleIt" ) ), mc ), mc );
		bridge.add( new PingTheSemanticWeb( new RdfValue( mc.createUri( ns + "pingTheSemanticWeb" ) ), mc ), mc );
	}
}

// kate: tab-width 4
