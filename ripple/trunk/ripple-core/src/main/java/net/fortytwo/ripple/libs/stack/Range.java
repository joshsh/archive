/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.*;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A primitive which activates ("applies") the topmost item on the stack one or
 * more times.
 */
public class Range extends PrimitiveFunction
{
	// TODO: arity should really be 2
	private static final int ARITY = 3;

	public Range() throws RippleException
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
		final int min, max;

		max = mc.toNumericValue( stack.getFirst() ).intValue();
		stack = stack.getRest();
		min = mc.toNumericValue( stack.getFirst() ).intValue();
		stack = stack.getRest();
		RippleValue p = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<Operator> opSink = new Sink<Operator>()
		{
			public void put( final Operator op ) throws RippleException
			{
				Function f = op.getFunction();

				sink.put( rest
						.push( new Operator(
								new TimesFunction( f, min, false ) ) )
						.push( new Operator(
								new TimesFunction( f, max - min, true ) ) ) );
			}
		};

		Operator.createOperator( p, opSink, mc );
	}
}
