package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public class TestExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/rpl-test#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new BagElements( ns + "bagElements" ) );
		bridge.add( new IntegerAdd( ns + "integer-add" ) );
		bridge.add( new IntegerSubtract( ns + "integer-subtract" ) );
		bridge.add( new IntegerMultiply( ns + "integer-multiply" ) );
		bridge.add( new IntegerDivide( ns + "integer-divide" ) );
		bridge.add( new IntegerMod( ns + "integer-mod" ) );
		bridge.add( new ConcatenateStringsPrimitive( ns + "concatenateStrings" ) );
		bridge.add( new ListElements( ns + "listElements" ) );
/*
			new Combinator_B( mc ),
			new Combinator_C( mc ),
			new Combinator_I( mc ),
			new Combinator_K( mc ),
			new Combinator_L( mc ),
			new Combinator_O( mc ),
			new Combinator_R( mc ),
			new Combinator_S( mc ),
			new Combinator_T( mc ),
			new Combinator_U( mc ),
			new Combinator_V( mc ),
			new Combinator_W( mc ),
			new Combinator_Y( mc ),
			new Combinator_w( mc )
*/
	}
}

// kate: tab-width 4
