package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class GraphExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/graph#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "graph.ttl" ) + "#" );

		registerPrimitive( Assert.class, ns + "assert", mc );
		registerPrimitive( Contains.class, ns + "contains", mc );
//		registerPrimitive( Count.class, ns + "count", mc );
		registerPrimitive( Compare.class, ns + "compare", mc );
		registerPrimitive( Deny.class, ns + "deny", mc );
		registerPrimitive( Equal.class, ns + "equal", mc );
		registerPrimitive( Forget.class, ns + "forget", mc );
		registerPrimitive( New.class, ns + "new", mc );

		// Type conversion / literal reification.
		registerPrimitive( ToDouble.class, ns + "toDouble", mc );
		registerPrimitive( ToInteger.class, ns + "toInteger", mc );
		registerPrimitive( ToString.class, ns + "toString", mc );
		registerPrimitive( ToUri.class, ns + "toUri", mc );

		// Document-centric primitives.
		registerPrimitive( Comments.class, ns + "comments", mc );
		registerPrimitive( Namespaces.class, ns + "namespaces", mc );
		registerPrimitive( Triples.class, ns + "triples", mc );
	}
}

// kate: tab-width 4
