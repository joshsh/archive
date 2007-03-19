package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public class NewExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/rpl-new#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		True truePrim;
		False falsePrim;
		bridge.add( truePrim = new True( ns + "true", mc ) );
		bridge.add( falsePrim = new False( ns + "false", mc ) );
		bridge.add( new IntegerGreaterThan( ns + "integer-greaterThan", mc, truePrim, falsePrim ) );
		bridge.add( new IntegerLessThan( ns + "integer-lessThan", mc, truePrim, falsePrim ) );
		bridge.add( new Assert( ns + "assert", mc ) );
		bridge.add( new Deny( ns + "deny", mc ) );
		bridge.add( new Equal( ns + "equal", mc ) );
		bridge.add( new Or( ns + "or", mc ) );
		bridge.add( new Dup( ns + "dup", mc ) );
		bridge.add( new Zap( ns + "zap", mc ) );
		bridge.add( new Scrap( ns + "scrap", mc ) );
		bridge.add( new Limit( ns + "limit", mc ) );
//        bridge.add( new Unique( ns + "unique", mc ) );
		bridge.add( new Swap( ns + "swap", mc ) );
	}
}

// kate: tab-width 4
