/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.libs.stack.StackLibrary;

/**
 * A primitive which consumes two items and produces a Boolean value of true if
 * they are equal according to their data types, otherwise false.
 */
public class Equal extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Equal()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue a, b, result;

		a = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		// Note: equals() is not suitable for this operation (for instance,
		//       it may yield false for RdfValues containing identical
		//       Literals).
		result = ( 0 == a.compareTo( b ) )
			? StackLibrary.getTrueValue()
			: StackLibrary.getFalseValue();

		sink.put( mc.list( result, stack ) );
	}
}

// kate: tab-width 4
