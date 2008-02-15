/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Library;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackRelation;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.UriMap;

/**
 * A collection of stack manipulation primitives.  Compare with Joy and other
 * functional stack languages.
 */
public class StackLibrary extends Library
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/stack#";

	// Some special values.
	private static RippleValue branchVal, trueVal, falseVal;
	private static PrimitiveStackRelation ioptVal, istarVal, iplusVal, timesVal, rangeVal;

	public void load( final UriMap uf, final ModelConnection mc )
		throws RippleException
	{
		uf.put(
			NS, getClass().getResource( "stack.ttl" ) + "#" );

		registerPrimitive( Ary.class, NS + "ary", mc );
		registerPrimitive( At.class, NS + "at", mc );
		branchVal = registerPrimitive( Branch.class, NS + "branch", mc );
		registerPrimitive( Choice.class, NS + "choice", mc );
		registerPrimitive( Dip.class, NS + "dip", mc );
		registerPrimitive( Dipd.class, NS + "dipd", mc );
		registerPrimitive( Dup.class, NS + "dup", mc );
		registerPrimitive( Dupd.class, NS + "dupd", mc );
		registerPrimitive( Dupdd.class, NS + "dupdd", mc );
		registerPrimitive( I.class, NS + "i", mc );
		registerPrimitive( Id.class, NS + "id", mc );
		registerPrimitive( Ifte.class, NS + "ifte", mc );
		registerPrimitive( Pop.class, NS + "pop", mc );
		registerPrimitive( Popd.class, NS + "popd", mc );
		registerPrimitive( Popdd.class, NS + "popdd", mc );
		registerPrimitive( Rolldown.class, NS + "rolldown", mc );
		registerPrimitive( Rolldownd.class, NS + "rolldownd", mc );
		registerPrimitive( Rollup.class, NS + "rollup", mc );
		registerPrimitive( Rollupd.class, NS + "rollupd", mc );
		registerPrimitive( Rotate.class, NS + "rotate", mc );
		registerPrimitive( Rotated.class, NS + "rotated", mc );
		registerPrimitive( Stack.class, NS + "stack", mc );
		registerPrimitive( Swap.class, NS + "swap", mc );
		registerPrimitive( Swapd.class, NS + "swapd", mc );
		registerPrimitive( Swapdd.class, NS + "swapdd", mc );
		registerPrimitive( Unstack.class, NS + "unstack", mc );
		registerPrimitive( X.class, NS + "x", mc );

		// Boolean logic
		registerPrimitive( And.class, NS + "and", mc );
		falseVal = registerPrimitive( False.class, NS + "false", mc );
		registerPrimitive( Not.class, NS + "not", mc );
		registerPrimitive( Or.class, NS + "or", mc );
		trueVal = registerPrimitive( True.class, NS + "true", mc );
		registerPrimitive( Xor.class, NS + "xor", mc );
		
		// "Quantifier" application primitives
// TODO: stack:times belongs here as well
// TODO: create RDF descriptions
		ioptVal = registerPrimitive( IOpt.class, NS + "iopt", mc );
		istarVal = registerPrimitive( IStar.class, NS + "istar", mc );
		iplusVal = registerPrimitive( IPlus.class, NS + "iplus", mc );
		rangeVal = registerPrimitive( Range.class, NS + "range", mc );
		timesVal = registerPrimitive( Times.class, NS + "times", mc );

		// List primitives.
		registerPrimitive( Cat.class, NS + "cat", mc );
		registerPrimitive( Cons.class, NS + "cons", mc );
		registerPrimitive( Fold.class, NS + "fold", mc );
		registerPrimitive( Has.class, NS + "has", mc );
		registerPrimitive( In.class, NS + "in", mc );
		registerPrimitive( Max.class, NS + "max", mc );
		registerPrimitive( Min.class, NS + "min", mc );
		registerPrimitive( Of.class, NS + "of", mc );
		registerPrimitive( Size.class, NS + "size", mc );
		registerPrimitive( Swons.class, NS + "swons", mc );
		registerPrimitive( Uncons.class, NS + "uncons", mc );
		registerPrimitive( Unswons.class, NS + "unswons", mc );
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

	public static PrimitiveStackRelation getIoptValue()
	{
		return ioptVal;
	}

	public static PrimitiveStackRelation getIstarValue()
	{
		return istarVal;
	}

	public static PrimitiveStackRelation getIplusValue()
	{
		return iplusVal;
	}

	public static PrimitiveStackRelation getTimesValue()
	{
		return timesVal;
	}

	public static PrimitiveStackRelation getRangeValue()
	{
		return rangeVal;
	}
}

// kate: tab-width 4
