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
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a resource or literal value and produces its
 * string representation.  For literal values, this is the same literal
 * value but with a type of xsd:string.  For resources identified by URIs,
 * this is the URI as a string.  For blank nodes, this is the identifier of
 * the node.
 */
public class ToString extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public ToString()
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
		RippleValue v;

		v = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( mc.value( v.toString() ), stack ) );
	}
}

// kate: tab-width 4
