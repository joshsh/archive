/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveStackRelation;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which produces the entire stack as a list.
 */
public class Stack extends PrimitiveStackRelation
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink ) throws RippleException
	{
		RippleList stack = arg.getStack();

// TODO: this should be even simpler.
		if ( RippleList.NIL != stack )
		{
			sink.put( arg.with(
					stack.push( stack ) ) );
		}
	}
}

// kate: tab-width 4
