/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.flow.Sink;

import org.openrdf.model.URI;

/**
 * A primitive which consumes a literal value and produces the resource
 * identified by the corresponding URI (if any).
 */
public class ToUri extends PrimitiveStackMapping
{
	private static final int ARITY = 1;

	public ToUri()
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
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		String s;

		s = mc.toString( stack.getFirst() );
		stack = stack.getRest();

		URI uri = mc.createUri( s );

		sink.put( arg.with(
				stack.push( mc.value( uri ) ) ) );
	}
}

// kate: tab-width 4
