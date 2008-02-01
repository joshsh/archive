/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a Boolean filter b, a filter t, and a filter t,
 * then applies an active copy of b to the stack.  If b yields a value of
 * true, then t is applied the rest of the stack.  Otherwise, f is applied to
 * the rest of the stack.
 */
public class Ifte extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public Ifte()
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
						final Context context )
		throws RippleException
	{
		RippleValue b, trueProg, falseProg;

		falseProg = stack.getFirst();
		stack = stack.getRest();
		trueProg = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		sink.put( stack.push( b )
			.push( Operator.OP )
			.push( trueProg )
			.push( falseProg )
			.push( StackLibrary.getBranchValue() )
			.push( Operator.OP ) );
	}
}

// kate: tab-width 4
