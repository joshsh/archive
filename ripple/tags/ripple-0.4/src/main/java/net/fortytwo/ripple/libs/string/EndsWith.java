/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.libs.stack.StackLibrary;

/**
 * A primitive which consumes a string and suffix, producing a Boolean value of
 * true if the given string ends with the given suffix, otherwise false.
 */
public class EndsWith extends PrimitiveFunction
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

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		String affix, s;
		RippleValue result;

		affix = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = ( s.endsWith( affix ) )
			? StackLibrary.getTrueValue()
			: StackLibrary.getFalseValue();
		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
