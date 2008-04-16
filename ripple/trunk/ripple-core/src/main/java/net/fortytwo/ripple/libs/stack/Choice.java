/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.flow.Sink;

/**
 * A primitive which consumes a Boolean value b, an item t, and an item f, then
 * produces t if b is true, otherwise f.
 */
public class Choice extends PrimitiveStackMapping
{
	private static final int ARITY = 3;

	public Choice()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink
	)
		throws RippleException
	{
		RippleValue f, t, b;
		RippleList stack = arg.getStack();

		f = stack.getFirst();
		stack = stack.getRest();
		t = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		// Note: everything apart from joy:true is considered false.
		RippleValue result = b.equals( StackLibrary.getTrueValue() ) ? t : f;

		sink.put( arg.with(
				stack.push( result ) ) );
	}
}

// kate: tab-width 4
