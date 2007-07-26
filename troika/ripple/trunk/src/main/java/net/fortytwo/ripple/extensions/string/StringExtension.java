package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class StringExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/07/ripple/string#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "string.ttl" ) + "#" );

		registerPrimitive( Concat.class, ns + "concat", mc );
		registerPrimitive( EndsWith.class, ns + "endsWith", mc );
		registerPrimitive( Length.class, ns + "length", mc );
		registerPrimitive( Matches.class, ns + "matches", mc );
		registerPrimitive( PercentEncode.class, ns + "percentEncode", mc );
		registerPrimitive( ReplaceAll.class, ns + "replaceAll", mc );
		registerPrimitive( Sha1.class, ns + "sha1", mc );
		registerPrimitive( Split.class, ns + "split", mc );
		registerPrimitive( StartsWith.class, ns + "startsWith", mc );
		registerPrimitive( Substring.class, ns + "substring", mc );
		registerPrimitive( ToLowerCase.class, ns + "toLowerCase", mc );
		registerPrimitive( ToUpperCase.class, ns + "toUpperCase", mc );
		registerPrimitive( Trim.class, ns + "trim", mc );
		registerPrimitive( UrlEncode.class, ns + "urlEncode", mc );
	}
}

// kate: tab-width 4
