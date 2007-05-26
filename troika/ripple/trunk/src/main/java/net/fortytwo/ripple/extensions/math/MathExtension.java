package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

public class MathExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/math#";

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		// Integer comparison primitives
		bridge.add( new Gt( new RdfValue( mc.createUri( ns + "gt" ) ), mc ), mc );
		bridge.add( new Lt( new RdfValue( mc.createUri( ns + "lt" ) ), mc ), mc );

		// Integer arithmetic primitives
		bridge.add( new Abs( new RdfValue( mc.createUri( ns + "abs" ) ), mc ), mc );
		bridge.add( new Add( new RdfValue( mc.createUri( ns + "add" ) ), mc ), mc );
		bridge.add( new Div( new RdfValue( mc.createUri( ns + "div" ) ), mc ), mc );
		bridge.add( new Mod( new RdfValue( mc.createUri( ns + "mod" ) ), mc ), mc );
		bridge.add( new Mul( new RdfValue( mc.createUri( ns + "mul" ) ), mc ), mc );
		bridge.add( new Neg( new RdfValue( mc.createUri( ns + "neg" ) ), mc ), mc );
		bridge.add( new Pow( new RdfValue( mc.createUri( ns + "pow" ) ), mc ), mc );
		bridge.add( new Sign( new RdfValue( mc.createUri( ns + "sign" ) ), mc ), mc );
		bridge.add( new Sub( new RdfValue( mc.createUri( ns + "sub" ) ), mc ), mc );
	}
}

// kate: tab-width 4
