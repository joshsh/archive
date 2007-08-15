/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class MathExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/08/ripple/math#";

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "math.ttl" ) + "#" );

		// Comparison
		registerPrimitive( Gt.class, ns + "gt", mc );
		registerPrimitive( Lt.class, ns + "lt", mc );

		// Arithmetic
		registerPrimitive( Abs.class, ns + "abs", mc );
		registerPrimitive( Add.class, ns + "add", mc );
		registerPrimitive( Div.class, ns + "div", mc );
		registerPrimitive( Mod.class, ns + "mod", mc );
		registerPrimitive( Mul.class, ns + "mul", mc );
		registerPrimitive( Neg.class, ns + "neg", mc );
		registerPrimitive( Signum.class, ns + "signum", mc );
		registerPrimitive( Sub.class, ns + "sub", mc );

		// Exponents
		registerPrimitive( Cbrt.class, ns + "cbrt", mc );
		registerPrimitive( Exp.class, ns + "exp", mc );
		registerPrimitive( Log.class, ns + "log", mc );
		registerPrimitive( Log10.class, ns + "log10", mc );
		registerPrimitive( Pow.class, ns + "pow", mc );
		registerPrimitive( Sqrt.class, ns + "sqrt", mc );

		// Trigonometry
		registerPrimitive( Acos.class, ns + "acos", mc );
		registerPrimitive( Asin.class, ns + "asin", mc );
		registerPrimitive( Atan.class, ns + "atan", mc );
		registerPrimitive( Cos.class, ns + "cos", mc );
		registerPrimitive( Cosh.class, ns + "cosh", mc );
		registerPrimitive( Sin.class, ns + "sin", mc );
		registerPrimitive( Sinh.class, ns + "sinh", mc );
		registerPrimitive( Tan.class, ns + "tan", mc );
		registerPrimitive( Tanh.class, ns + "tanh", mc );

		// Misc
		registerPrimitive( Random.class, ns + "random", mc );
	}
}

// kate: tab-width 4
