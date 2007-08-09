package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class StringExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/08/ripple/string#";

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "string.ttl" ) + "#" );

		registerPrimitive( EndsWith.class, ns + "endsWith", mc );
		registerPrimitive( IndexOf.class, ns + "indexOf", mc );
		registerPrimitive( LastIndexOf.class, ns + "lastIndexOf", mc );
		registerPrimitive( Length.class, ns + "length", mc );
		registerPrimitive( Matches.class, ns + "matches", mc );
		registerPrimitive( PercentEncode.class, ns + "percentEncode", mc );
		registerPrimitive( ReplaceAll.class, ns + "replaceAll", mc );
		registerPrimitive( Sha1.class, ns + "sha1", mc );
		registerPrimitive( Split.class, ns + "split", mc );
		registerPrimitive( StartsWith.class, ns + "startsWith", mc );
		registerPrimitive( StrCat.class, ns + "strCat", mc );
		registerPrimitive( Substring.class, ns + "substring", mc );
		registerPrimitive( ToLowerCase.class, ns + "toLowerCase", mc );
		registerPrimitive( ToUpperCase.class, ns + "toUpperCase", mc );
		registerPrimitive( Trim.class, ns + "trim", mc );
		registerPrimitive( UrlEncode.class, ns + "urlEncode", mc );
	}
}

// kate: tab-width 4
