package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Extension;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;

public class StackExtension implements Extension
{
	private static String ns = "http://fortytwo.net/2007/05/ripple/stack#";

	// Some special values.
	private static RippleValue branchVal, trueVal, falseVal;

	public void load( ModelConnection mc )
		throws RippleException
	{
		ModelBridge bridge = mc.getModel().getBridge();

		bridge.add( new And( new RdfValue( mc.createUri( ns + "and" ) ), mc ), mc );
		bridge.add( new Ary( new RdfValue( mc.createUri( ns + "ary" ) ), mc ), mc );
		bridge.add( new At( new RdfValue( mc.createUri( ns + "at" ) ), mc ), mc );
		bridge.add( branchVal = new Branch( new RdfValue( mc.createUri( ns + "branch" ) ), mc ), mc );
		bridge.add( new Choice( new RdfValue( mc.createUri( ns + "choice" ) ), mc ), mc );
		bridge.add( new Cat( new RdfValue( mc.createUri( ns + "cat" ) ), mc ), mc );
		bridge.add( new Cons( new RdfValue( mc.createUri( ns + "cons" ) ), mc ), mc );
		bridge.add( new Dip( new RdfValue( mc.createUri( ns + "dip" ) ), mc ), mc );
		bridge.add( new Dipd( new RdfValue( mc.createUri( ns + "dipd" ) ), mc ), mc );
		bridge.add( new Dup( new RdfValue( mc.createUri( ns + "dup" ) ), mc ), mc );
		bridge.add( new Dupd( new RdfValue( mc.createUri( ns + "dupd" ) ), mc ), mc );
		bridge.add( falseVal = new False( new RdfValue( mc.createUri( ns + "false" ) ), mc ), mc );
		bridge.add( new Fold( new RdfValue( mc.createUri( ns + "fold" ) ), mc ), mc );
		bridge.add( new Has( new RdfValue( mc.createUri( ns + "has" ) ), mc ), mc );
		bridge.add( new I( new RdfValue( mc.createUri( ns + "i" ) ), mc ), mc );
		bridge.add( new Id( new RdfValue( mc.createUri( ns + "id" ) ), mc ), mc );
		bridge.add( new Ifte( new RdfValue( mc.createUri( ns + "ifte" ) ), mc ), mc );
		bridge.add( new In( new RdfValue( mc.createUri( ns + "in" ) ), mc ), mc );
		bridge.add( new Not( new RdfValue( mc.createUri( ns + "not" ) ), mc ), mc );
		bridge.add( new Of( new RdfValue( mc.createUri( ns + "of" ) ), mc ), mc );
		bridge.add( new Or( new RdfValue( mc.createUri( ns + "or" ) ), mc ), mc );
		bridge.add( new Pop( new RdfValue( mc.createUri( ns + "pop" ) ), mc ), mc );
		bridge.add( new Popd( new RdfValue( mc.createUri( ns + "popd" ) ), mc ), mc );
		bridge.add( new Rolldown( new RdfValue( mc.createUri( ns + "rolldown" ) ), mc ), mc );
		bridge.add( new Rolldownd( new RdfValue( mc.createUri( ns + "rolldownd" ) ), mc ), mc );
		bridge.add( new Rollup( new RdfValue( mc.createUri( ns + "rollup" ) ), mc ), mc );
		bridge.add( new Rollupd( new RdfValue( mc.createUri( ns + "rollupd" ) ), mc ), mc );
		bridge.add( new Rotate( new RdfValue( mc.createUri( ns + "rotate" ) ), mc ), mc );
		bridge.add( new Rotated( new RdfValue( mc.createUri( ns + "rotated" ) ), mc ), mc );
		bridge.add( new Size( new RdfValue( mc.createUri( ns + "size" ) ), mc ), mc );
		bridge.add( new Stack( new RdfValue( mc.createUri( ns + "stack" ) ), mc ), mc );
		bridge.add( new Swap( new RdfValue( mc.createUri( ns + "swap" ) ), mc ), mc );
		bridge.add( new Swapd( new RdfValue( mc.createUri( ns + "swapd" ) ), mc ), mc );
		bridge.add( new Swons( new RdfValue( mc.createUri( ns + "swons" ) ), mc ), mc );
		bridge.add( new Times( new RdfValue( mc.createUri( ns + "times" ) ), mc ), mc );
		bridge.add( trueVal = new True( new RdfValue( mc.createUri( ns + "true" ) ), mc ), mc );
		bridge.add( new Uncons( new RdfValue( mc.createUri( ns + "uncons" ) ), mc ), mc );
		bridge.add( new Unstack( new RdfValue( mc.createUri( ns + "unstack" ) ), mc ), mc );
		bridge.add( new Unswons( new RdfValue( mc.createUri( ns + "unswons" ) ), mc ), mc );
		bridge.add( new X( new RdfValue( mc.createUri( ns + "x" ) ), mc ), mc );
		bridge.add( new Xor( new RdfValue( mc.createUri( ns + "xor" ) ), mc ), mc );
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
