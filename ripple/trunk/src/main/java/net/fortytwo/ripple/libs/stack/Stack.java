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
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which produces the entire stack as a list.
 */
public class Stack extends PrimitiveFunction
{
	private static final int ARITY = 0;

	public Stack()
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
// TODO: this should be even simpler.
		if ( RippleList.NIL != stack )
		{
			sink.put( new RippleList( stack, stack ) );
		}
	}
}

// kate: tab-width 4
