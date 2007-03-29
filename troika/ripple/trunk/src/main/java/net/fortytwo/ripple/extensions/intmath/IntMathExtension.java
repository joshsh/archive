package net.fortytwo.ripple.extensions.intmath;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class IntMathExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/03/ripple/intmath#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		// Integer-specific comparison primitives
		bridge.add( new IntegerGreaterThan( new RdfValue( mc.createUri( ns + "gt" ) ), mc ), mc );
		bridge.add( new IntegerLessThan( new RdfValue( mc.createUri( ns + "lt" ) ), mc ), mc );

		// Integer arithmetic primitives
		bridge.add( new IntegerAbs( new RdfValue( mc.createUri( ns + "abs" ) ), mc ), mc );
		bridge.add( new IntegerAdd( new RdfValue( mc.createUri( ns + "add" ) ), mc ), mc );
		bridge.add( new IntegerSubtract( new RdfValue( mc.createUri( ns + "sub" ) ), mc ), mc );
		bridge.add( new IntegerMultiply( new RdfValue( mc.createUri( ns + "mul" ) ), mc ), mc );
		bridge.add( new IntegerDivide( new RdfValue( mc.createUri( ns + "div" ) ), mc ), mc );
		bridge.add( new IntegerMod( new RdfValue( mc.createUri( ns + "mod" ) ), mc ), mc );
	}
}

// kate: tab-width 4
