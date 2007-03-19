package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public class MiscExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/rpl-misc#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new Sha1SumOf( ns + "sha1SumOf" ) );
		bridge.add( new UriToString( ns + "uriToString" ) );
		bridge.add( new UrlEncoding( ns + "urlEncoding" ) );
		bridge.add( new UrlTarget( ns + "urlTarget" ) );
		bridge.add( new SwoogleIt( ns + "swoogleIt" ) );
		bridge.add( new PingTheSemanticWeb( ns + "pingTheSemanticWeb" ) );
	}
}

// kate: tab-width 4
