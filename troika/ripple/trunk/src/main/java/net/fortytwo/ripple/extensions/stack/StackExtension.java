package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Extension;
import net.fortytwo.ripple.util.UrlFactory;

public class StackExtension extends Extension
{
	private static String ns = "http://fortytwo.net/2007/08/ripple/stack#";

	// Some special values.
	private static RippleValue branchVal, trueVal, falseVal;

	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException
	{
		uf.addMapping(
			ns, getClass().getResource( "stack.ttl" ) + "#" );

		registerPrimitive( And.class, ns + "and", mc );
		registerPrimitive( Ary.class, ns + "ary", mc );
		registerPrimitive( At.class, ns + "at", mc );
		branchVal = registerPrimitive( Branch.class, ns + "branch", mc );
		registerPrimitive( Choice.class, ns + "choice", mc );
		registerPrimitive( Dip.class, ns + "dip", mc );
		registerPrimitive( Dipd.class, ns + "dipd", mc );
		registerPrimitive( Dup.class, ns + "dup", mc );
		registerPrimitive( Dupd.class, ns + "dupd", mc );
		registerPrimitive( Dupdd.class, ns + "dupdd", mc );
		falseVal = registerPrimitive( False.class, ns + "false", mc );
		registerPrimitive( I.class, ns + "i", mc );
		registerPrimitive( Id.class, ns + "id", mc );
		registerPrimitive( Ifte.class, ns + "ifte", mc );
		registerPrimitive( Not.class, ns + "not", mc );
		registerPrimitive( Or.class, ns + "or", mc );
		registerPrimitive( Pop.class, ns + "pop", mc );
		registerPrimitive( Popd.class, ns + "popd", mc );
		registerPrimitive( Popdd.class, ns + "popdd", mc );
		registerPrimitive( Rolldown.class, ns + "rolldown", mc );
		registerPrimitive( Rolldownd.class, ns + "rolldownd", mc );
		registerPrimitive( Rollup.class, ns + "rollup", mc );
		registerPrimitive( Rollupd.class, ns + "rollupd", mc );
		registerPrimitive( Rotate.class, ns + "rotate", mc );
		registerPrimitive( Rotated.class, ns + "rotated", mc );
		registerPrimitive( Stack.class, ns + "stack", mc );
		registerPrimitive( Swap.class, ns + "swap", mc );
		registerPrimitive( Swapd.class, ns + "swapd", mc );
		registerPrimitive( Swapdd.class, ns + "swapdd", mc );
		registerPrimitive( Times.class, ns + "times", mc );
		trueVal = registerPrimitive( True.class, ns + "true", mc );
		registerPrimitive( Unstack.class, ns + "unstack", mc );
		registerPrimitive( X.class, ns + "x", mc );
		registerPrimitive( Xor.class, ns + "xor", mc );

		// List primitives.
		registerPrimitive( Cat.class, ns + "cat", mc );
		registerPrimitive( Cons.class, ns + "cons", mc );
		registerPrimitive( Fold.class, ns + "fold", mc );
		registerPrimitive( Has.class, ns + "has", mc );
		registerPrimitive( In.class, ns + "in", mc );
		registerPrimitive( Max.class, ns + "max", mc );
		registerPrimitive( Min.class, ns + "min", mc );
		registerPrimitive( Of.class, ns + "of", mc );
		registerPrimitive( Size.class, ns + "size", mc );
		registerPrimitive( Swons.class, ns + "swons", mc );
		registerPrimitive( Uncons.class, ns + "uncons", mc );
		registerPrimitive( Unswons.class, ns + "unswons", mc );
	}

	////////////////////////////////////////////////////////////////////////////

	public static RippleValue getBranchValue()
	{
		return branchVal;
	}

	public static RippleValue getTrueValue()
	{
		return trueVal;
	}

	public static RippleValue getFalseValue()
	{
		return falseVal;
	}
}

// kate: tab-width 4
