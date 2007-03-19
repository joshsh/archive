package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.Extension;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.RdfValue;

public class TestExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/rpl-test#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new BagElements( new RdfValue( mc.createUri( ns + "bagElements" ) ), mc ), mc );
		bridge.add( new IntegerAdd( new RdfValue( mc.createUri( ns + "integer-add" ) ), mc ), mc );
		bridge.add( new IntegerSubtract( new RdfValue( mc.createUri( ns + "integer-subtract" ) ), mc ), mc );
		bridge.add( new IntegerMultiply( new RdfValue( mc.createUri( ns + "integer-multiply" ) ), mc ), mc );
		bridge.add( new IntegerDivide( new RdfValue( mc.createUri( ns + "integer-divide" ) ), mc ), mc );
		bridge.add( new IntegerMod( new RdfValue( mc.createUri( ns + "integer-mod" ) ), mc ), mc );
		bridge.add( new ConcatenateStringsPrimitive( new RdfValue( mc.createUri( ns + "concatenateStrings" ) ), mc ), mc );
		bridge.add( new ListElements( new RdfValue( mc.createUri( ns + "listElements" ) ), mc ), mc );
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
