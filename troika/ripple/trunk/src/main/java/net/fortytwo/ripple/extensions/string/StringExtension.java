package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.UrlFactory;

public class StringExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/string#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "string.ttl" ) + "#" );

		bridge.add( new Concat( new RdfValue( mc.createUri( ns + "concat" ) ), mc ), mc );
		bridge.add( new EndsWith( new RdfValue( mc.createUri( ns + "endsWith" ) ), mc ), mc );
		bridge.add( new Length( new RdfValue( mc.createUri( ns + "length" ) ), mc ), mc );
		bridge.add( new Matches( new RdfValue( mc.createUri( ns + "matches" ) ), mc ), mc );
		bridge.add( new ReplaceAll( new RdfValue( mc.createUri( ns + "replaceAll" ) ), mc ), mc );
		bridge.add( new Sha1( new RdfValue( mc.createUri( ns + "sha1" ) ), mc ), mc );
		bridge.add( new Split( new RdfValue( mc.createUri( ns + "split" ) ), mc ), mc );
		bridge.add( new StartsWith( new RdfValue( mc.createUri( ns + "startsWith" ) ), mc ), mc );
		bridge.add( new Substring( new RdfValue( mc.createUri( ns + "substring" ) ), mc ), mc );
		bridge.add( new ToLowerCase( new RdfValue( mc.createUri( ns + "toLowerCase" ) ), mc ), mc );
		bridge.add( new ToUpperCase( new RdfValue( mc.createUri( ns + "toUpperCase" ) ), mc ), mc );
		bridge.add( new Trim( new RdfValue( mc.createUri( ns + "trim" ) ), mc ), mc );
		bridge.add( new UrlEncoding( new RdfValue( mc.createUri( ns + "urlEncoding" ) ), mc ), mc );
	}
}

// kate: tab-width 4
