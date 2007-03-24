package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class JoyExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/ripple/joy#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new Dup( new RdfValue( mc.createUri( ns + "dup" ) ), mc ), mc );
		bridge.add( new False( new RdfValue( mc.createUri( ns + "false" ) ), mc ), mc );
		bridge.add( new Fold( new RdfValue( mc.createUri( ns + "fold" ) ), mc ), mc );
		bridge.add( new Ifte( new RdfValue( mc.createUri( ns + "ifte" ) ), mc ), mc );
		bridge.add( new Map( new RdfValue( mc.createUri( ns + "map" ) ), mc ), mc );
		bridge.add( new I( new RdfValue( mc.createUri( ns + "i" ) ), mc ), mc );
		bridge.add( new True( new RdfValue( mc.createUri( ns + "true" ) ), mc ), mc );
		bridge.add( new Swap( new RdfValue( mc.createUri( ns + "swap" ) ), mc ), mc );
		bridge.add( new Zap( new RdfValue( mc.createUri( ns + "zap" ) ), mc ), mc );
	}
}

// kate: tab-width 4
