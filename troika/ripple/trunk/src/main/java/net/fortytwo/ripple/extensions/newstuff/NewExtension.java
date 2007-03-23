package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class NewExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/rpl-new#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		True truePrim;
		False falsePrim;

		bridge.add( new Pred( new RdfValue( mc.createUri( ns + "pred" ) ), mc ), mc );
		bridge.add( new Prim( new RdfValue( mc.createUri( ns + "prim" ) ), mc ), mc );
		bridge.add( new Prog( new RdfValue( mc.createUri( ns + "prog" ) ), mc ), mc );

		bridge.add( truePrim = new True( new RdfValue( mc.createUri( ns + "true" ) ), mc ), mc );
		bridge.add( falsePrim = new False( new RdfValue( mc.createUri( ns + "false" ) ), mc ), mc );
		bridge.add( new Assert( new RdfValue( mc.createUri( ns + "assert" ) ), mc ), mc );
		bridge.add( new Deny( new RdfValue( mc.createUri( ns + "deny" ) ), mc ), mc );
		bridge.add( new Equal( new RdfValue( mc.createUri( ns + "equal" ) ), mc, truePrim, falsePrim ), mc );
		bridge.add( new Or( new RdfValue( mc.createUri( ns + "or" ) ), mc ), mc );
		bridge.add( new Dup( new RdfValue( mc.createUri( ns + "dup" ) ), mc ), mc );
		bridge.add( new Zap( new RdfValue( mc.createUri( ns + "zap" ) ), mc ), mc );
		bridge.add( new Scrap( new RdfValue( mc.createUri( ns + "scrap" ) ), mc ), mc );
//		bridge.add( new Count( new RdfValue( mc.createUri( ns + "count" ) ), mc ), mc );
		bridge.add( new Limit( new RdfValue( mc.createUri( ns + "limit" ) ), mc ), mc );
        bridge.add( new Unique( new RdfValue( mc.createUri( ns + "unique" ) ), mc ), mc );
		bridge.add( new Swap( new RdfValue( mc.createUri( ns + "swap" ) ), mc ), mc );
		bridge.add( new Forget( new RdfValue( mc.createUri( ns + "forget" ) ), mc ), mc );
		bridge.add( new New( new RdfValue( mc.createUri( ns + "new" ) ), mc ), mc );
		bridge.add( new Times( new RdfValue( mc.createUri( ns + "times" ) ), mc ), mc );
		bridge.add( new Ifte( new RdfValue( mc.createUri( ns + "ifte" ) ), mc, truePrim, falsePrim ), mc );
	}
}

// kate: tab-width 4
