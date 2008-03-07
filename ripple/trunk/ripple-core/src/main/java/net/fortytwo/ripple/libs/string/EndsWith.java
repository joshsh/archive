/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.libs.stack.StackLibrary;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a string and suffix, producing a Boolean value of
 * true if the given string ends with the given suffix, otherwise false.
 */
public class EndsWith extends PrimitiveStackMapping
{
	private static final int ARITY = 2;

	public EndsWith()
		throws RippleException
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

		String affix, s;
		RippleValue result;

		affix = mc.toString( stack.getFirst() );
		stack = stack.getRest();
		s = mc.toString( stack.getFirst() );
		stack = stack.getRest();

		result = ( s.endsWith( affix ) )
			? StackLibrary.getTrueValue()
			: StackLibrary.getFalseValue();
		sink.put( arg.with(
				stack.push( result ) ) );
	}
}

// kate: tab-width 4
