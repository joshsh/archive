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
public class IPlus extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public IPlus() throws RippleException
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
		RippleValue first = stack.getFirst();

// hack...
		sink.put( stack
				.push( Operator.OP )
				.push( first )
				.push( new Operator( StackLibrary.getIstarValue() ) ) );
	}
}