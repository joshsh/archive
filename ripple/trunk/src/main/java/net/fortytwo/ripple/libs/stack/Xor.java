/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Xor extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Xor()
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
		RippleValue x, y;

		x = stack.getFirst();
		stack = stack.getRest();
		y = stack.getFirst();
		stack = stack.getRest();

		RippleValue trueValue = StackLibrary.getTrueValue();

		// Note: everything apart from joy:true is considered false.
		boolean xb = ( 0 == x.compareTo( trueValue ) );
		boolean yb = ( 0 == y.compareTo( trueValue ) );
		RippleValue result = ( ( xb && !yb ) || ( !xb && yb ) )
			? trueValue
			: StackLibrary.getFalseValue();

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
