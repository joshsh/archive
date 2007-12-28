/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;

/**
 * A primitive which consumes a literal value and produces the resource
 * identified by the corresponding URI (if any).
 */
public class ToUri extends PrimitiveFunction
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

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		URI uri = mc.createUri( s );

		sink.put( mc.list(
			mc.value( uri ), stack ) );
	}
}

// kate: tab-width 4
