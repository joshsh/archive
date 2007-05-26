package net.fortytwo.ripple.extensions.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class StreamExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/stream#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new Each( new RdfValue( mc.createUri( ns + "each" ) ), mc ), mc );
		bridge.add( new Limit( new RdfValue( mc.createUri( ns + "limit" ) ), mc ), mc );
		bridge.add( new Scrap( new RdfValue( mc.createUri( ns + "scrap" ) ), mc ), mc );
		bridge.add( new Union( new RdfValue( mc.createUri( ns + "union" ) ), mc ), mc );
		bridge.add( new Unique( new RdfValue( mc.createUri( ns + "unique" ) ), mc ), mc );
	}
}

// kate: tab-width 4
