package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class GraphExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/03/ripple/graph#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new Assert( new RdfValue( mc.createUri( ns + "assert" ) ), mc ), mc );
		bridge.add( new BagElements( new RdfValue( mc.createUri( ns + "bagElements" ) ), mc ), mc );
//		bridge.add( new Count( new RdfValue( mc.createUri( ns + "count" ) ), mc ), mc );
		bridge.add( new Deny( new RdfValue( mc.createUri( ns + "deny" ) ), mc ), mc );
		bridge.add( new Equal( new RdfValue( mc.createUri( ns + "equal" ) ), mc ), mc );
		bridge.add( new Forget( new RdfValue( mc.createUri( ns + "forget" ) ), mc ), mc );
		bridge.add( new New( new RdfValue( mc.createUri( ns + "new" ) ), mc ), mc );
		bridge.add( new ToString( new RdfValue( mc.createUri( ns + "toString" ) ), mc ), mc );
	}
}

// kate: tab-width 4
