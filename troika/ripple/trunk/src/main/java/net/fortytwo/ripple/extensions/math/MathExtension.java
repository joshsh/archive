package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class MathExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/math#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();
		uf.addMapping(
			ns, getClass().getResource( "math.ttl" ) + "#" );

		// Integer comparison primitives
		registerPrimitive( Gt.class, ns + "gt", mc );
		registerPrimitive( Lt.class, ns + "lt", mc );

		// Integer arithmetic primitives
		registerPrimitive( Abs.class, ns + "abs", mc );
		registerPrimitive( Add.class, ns + "add", mc );
		registerPrimitive( Div.class, ns + "div", mc );
		registerPrimitive( Mod.class, ns + "mod", mc );
		registerPrimitive( Mul.class, ns + "mul", mc );
		registerPrimitive( Neg.class, ns + "neg", mc );
		registerPrimitive( Pow.class, ns + "pow", mc );
		registerPrimitive( Sign.class, ns + "sign", mc );
		registerPrimitive( Sub.class, ns + "sub", mc );
	}
}

// kate: tab-width 4
