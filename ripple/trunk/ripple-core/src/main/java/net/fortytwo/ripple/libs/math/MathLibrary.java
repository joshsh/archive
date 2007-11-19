/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Library;
import net.fortytwo.ripple.util.UrlFactory;

/**
 * A collection of mathematical primitives.
 */
public class MathLibrary extends Library
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/math#";

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			NS, getClass().getResource( "math.ttl" ) + "#" );

		// Comparison
		registerPrimitive( Gt.class, NS + "gt", mc );
		registerPrimitive( Lt.class, NS + "lt", mc );

		// Arithmetic
		registerPrimitive( Abs.class, NS + "abs", mc );
		registerPrimitive( Add.class, NS + "add", mc );
		registerPrimitive( Div.class, NS + "div", mc );
		registerPrimitive( Mod.class, NS + "mod", mc );
		registerPrimitive( Mul.class, NS + "mul", mc );
		registerPrimitive( Neg.class, NS + "neg", mc );
		registerPrimitive( Signum.class, NS + "signum", mc );
		registerPrimitive( Sub.class, NS + "sub", mc );

		// Exponents
		registerPrimitive( Cbrt.class, NS + "cbrt", mc );
		registerPrimitive( Exp.class, NS + "exp", mc );
		registerPrimitive( Log.class, NS + "log", mc );
		registerPrimitive( Log10.class, NS + "log10", mc );
		registerPrimitive( Pow.class, NS + "pow", mc );
		registerPrimitive( Sqrt.class, NS + "sqrt", mc );

		// Trigonometry
		registerPrimitive( Acos.class, NS + "acos", mc );
		registerPrimitive( Asin.class, NS + "asin", mc );
		registerPrimitive( Atan.class, NS + "atan", mc );
		registerPrimitive( Cos.class, NS + "cos", mc );
		registerPrimitive( Cosh.class, NS + "cosh", mc );
		registerPrimitive( Sin.class, NS + "sin", mc );
		registerPrimitive( Sinh.class, NS + "sinh", mc );
		registerPrimitive( Tan.class, NS + "tan", mc );
		registerPrimitive( Tanh.class, NS + "tanh", mc );

		// Misc
		registerPrimitive( Ceil.class, NS + "ceil", mc );
		registerPrimitive( Floor.class, NS + "floor", mc );
		registerPrimitive( Random.class, NS + "random", mc );
	}
}

// kate: tab-width 4
