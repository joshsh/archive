package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.UrlFactory;

public class GraphExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/graph#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "graph.ttl" ) + "#" );

		bridge.add( new Assert( new RdfValue( mc.createUri( ns + "assert" ) ), mc ), mc );
		bridge.add( new Contains( new RdfValue( mc.createUri( ns + "contains" ) ), mc ), mc );
//		bridge.add( new Count( new RdfValue( mc.createUri( ns + "count" ) ), mc ), mc );
		bridge.add( new Compare( new RdfValue( mc.createUri( ns + "compare" ) ), mc ), mc );
		bridge.add( new Deny( new RdfValue( mc.createUri( ns + "deny" ) ), mc ), mc );
		bridge.add( new Equal( new RdfValue( mc.createUri( ns + "equal" ) ), mc ), mc );
		bridge.add( new Forget( new RdfValue( mc.createUri( ns + "forget" ) ), mc ), mc );
		bridge.add( new New( new RdfValue( mc.createUri( ns + "new" ) ), mc ), mc );
		bridge.add( new ToDouble( new RdfValue( mc.createUri( ns + "toDouble" ) ), mc ), mc );
		bridge.add( new ToInteger( new RdfValue( mc.createUri( ns + "toInteger" ) ), mc ), mc );
		bridge.add( new ToString( new RdfValue( mc.createUri( ns + "toString" ) ), mc ), mc );
		bridge.add( new ToUri( new RdfValue( mc.createUri( ns + "toUri" ) ), mc ), mc );
	}
}

// kate: tab-width 4
