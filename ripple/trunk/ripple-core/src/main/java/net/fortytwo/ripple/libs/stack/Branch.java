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
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a Boolean value b, a filter t, and a filter f,
 * then produces an active copy of t if b is true, otherwise an active copy of
 * f.
 */
public class Branch extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public Branch()
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
		RippleValue b, trueProg, falseProg;

		falseProg = stack.getFirst();
		stack = stack.getRest();
		trueProg = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		RippleValue result;
		if ( b.equals( StackLibrary.getTrueValue() ) )
		{
			result = trueProg;
		}

		else if ( b.equals( StackLibrary.getFalseValue() ) )
		{
			result = falseProg;
		}

		else
		{
			throw new RippleException( "branch expects one of the values true, false as its third argument" );
		}

		sink.put( mc.list( result, stack ).push( Operator.OP ) );
	}
}

// kate: tab-width 4
