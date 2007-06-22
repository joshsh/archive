package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class EtcExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/etc#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "etc.ttl" ) + "#" );

		bridge.add( new Back( new RdfValue( mc.createUri( ns + "back" ) ), mc ), mc );
		bridge.add( new Get( new RdfValue( mc.createUri( ns + "get" ) ), mc ), mc );
		bridge.add( new PingTheSemanticWeb( new RdfValue( mc.createUri( ns + "pingTheSemanticWeb" ) ), mc ), mc );
		bridge.add( new Pred( new RdfValue( mc.createUri( ns + "pred" ) ), mc ), mc );
		bridge.add( new Prim( new RdfValue( mc.createUri( ns + "prim" ) ), mc ), mc );
		bridge.add( new Rand( new RdfValue( mc.createUri( ns + "rand" ) ), mc ), mc );
		bridge.add( new Sindice( new RdfValue( mc.createUri( ns + "sindice" ) ), mc ), mc );
		bridge.add( new SwoogleIt( new RdfValue( mc.createUri( ns + "swoogleIt" ) ), mc ), mc );
		bridge.add( new Time( new RdfValue( mc.createUri( ns + "time" ) ), mc ), mc );

		// Note: the xml: namespace is actually said to be
		//       http://www.w3.org/XML/1998/namespace
		//       (i.e. without the hash character).
		bridge.add( new Lang( new RdfValue( mc.createUri( "http://www.w3.org/XML/1998/namespace#lang" ) ), mc ), mc );

		// Note: this URI is bogus.
		bridge.add( new Type( new RdfValue( mc.createUri( "http://www.w3.org/2001/XMLSchema#type" ) ), mc ), mc );
	}
}

// kate: tab-width 4
