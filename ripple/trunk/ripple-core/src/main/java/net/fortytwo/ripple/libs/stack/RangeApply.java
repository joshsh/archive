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
import net.fortytwo.ripple.model.regex.TimesQuantifier;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A primitive which activates ("applies") the topmost item on the stack one or
 * more times.
 */
public class RangeApply extends PrimitiveStackMapping
{
	// TODO: arity should really be 2
	private static final int ARITY = 3;

	public RangeApply() throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		RippleList stack = arg.getStack();
		final ModelConnection mc = arg.getModelConnection();

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
				sink.put( arg.with( rest.push(
						new Operator( new TimesQuantifier( op, min, max ) ) ) ) );
			}
		};

		Operator.createOperator( p, opSink, mc );
	}
}
