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
import net.fortytwo.ripple.model.regex.PlusQuantifier;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A primitive which activates ("applies") the topmost item on the stack one or
 * more times.
 */
public class PlusApply extends PrimitiveStackMapping
{
	private static final int ARITY = 1;

	public PlusApply() throws RippleException
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
		RippleValue first = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<Operator> opSink = new Sink<Operator>()
		{
			public void put( final Operator op ) throws RippleException
			{
				sink.put( arg.with( rest.push(
						new Operator( new PlusQuantifier( op ) ) ) ) );
			}
		};

		Operator.createOperator( first, opSink, arg.getModelConnection() );
	}
}
