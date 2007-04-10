package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class JoyExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/03/ripple/joy#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new And( new RdfValue( mc.createUri( ns + "and" ) ), mc ), mc );
		bridge.add( new At( new RdfValue( mc.createUri( ns + "at" ) ), mc ), mc );
		bridge.add( new Concat( new RdfValue( mc.createUri( ns + "concat" ) ), mc ), mc );
		bridge.add( new Cons( new RdfValue( mc.createUri( ns + "cons" ) ), mc ), mc );
		bridge.add( new Dip( new RdfValue( mc.createUri( ns + "dip" ) ), mc ), mc );
		bridge.add( new Dup( new RdfValue( mc.createUri( ns + "dup" ) ), mc ), mc );
		bridge.add( new False( new RdfValue( mc.createUri( ns + "false" ) ), mc ), mc );
		bridge.add( new Fold( new RdfValue( mc.createUri( ns + "fold" ) ), mc ), mc );
		bridge.add( new I( new RdfValue( mc.createUri( ns + "i" ) ), mc ), mc );
		bridge.add( new Id( new RdfValue( mc.createUri( ns + "id" ) ), mc ), mc );
		bridge.add( new Ifte( new RdfValue( mc.createUri( ns + "ifte" ) ), mc ), mc );
		bridge.add( new Not( new RdfValue( mc.createUri( ns + "not" ) ), mc ), mc );
		bridge.add( new Or( new RdfValue( mc.createUri( ns + "or" ) ), mc ), mc );
		bridge.add( new Pop( new RdfValue( mc.createUri( ns + "pop" ) ), mc ), mc );
		bridge.add( new Size( new RdfValue( mc.createUri( ns + "size" ) ), mc ), mc );
		bridge.add( new Stack( new RdfValue( mc.createUri( ns + "stack" ) ), mc ), mc );
		bridge.add( new Swap( new RdfValue( mc.createUri( ns + "swap" ) ), mc ), mc );
		bridge.add( new Times( new RdfValue( mc.createUri( ns + "times" ) ), mc ), mc );
		bridge.add( new True( new RdfValue( mc.createUri( ns + "true" ) ), mc ), mc );
		bridge.add( new Xor( new RdfValue( mc.createUri( ns + "xor" ) ), mc ), mc );
	}
}

// kate: tab-width 4
