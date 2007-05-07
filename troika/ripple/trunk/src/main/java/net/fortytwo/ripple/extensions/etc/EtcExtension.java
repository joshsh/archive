package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class EtcExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/03/ripple/etc#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new PingTheSemanticWeb( new RdfValue( mc.createUri( ns + "pingTheSemanticWeb" ) ), mc ), mc );
		bridge.add( new Pred( new RdfValue( mc.createUri( ns + "pred" ) ), mc ), mc );
		bridge.add( new Prim( new RdfValue( mc.createUri( ns + "prim" ) ), mc ), mc );
		bridge.add( new Rand( new RdfValue( mc.createUri( ns + "rand" ) ), mc ), mc );
		bridge.add( new Sha1( new RdfValue( mc.createUri( ns + "sha1" ) ), mc ), mc );
		bridge.add( new StrCat( new RdfValue( mc.createUri( ns + "strCat" ) ), mc ), mc );
		bridge.add( new SwoogleIt( new RdfValue( mc.createUri( ns + "swoogleIt" ) ), mc ), mc );
		bridge.add( new Time( new RdfValue( mc.createUri( ns + "time" ) ), mc ), mc );
		bridge.add( new UrlEncoding( new RdfValue( mc.createUri( ns + "urlEncoding" ) ), mc ), mc );
		bridge.add( new UrlGet( new RdfValue( mc.createUri( ns + "urlGet" ) ), mc ), mc );
	}
}

// kate: tab-width 4
