/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/libs/graph/Back.java $
 * $Revision: 928 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfPredicateFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which follows inferred forward triples from a resource.
 */
public class Infer extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Infer()
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
		RippleValue pred;

		pred = stack.getFirst();
		stack = stack.getRest();

		sink.put( stack.push( new Operator( new RdfPredicateFunction( pred.toRdf( mc ), true ) ) ) );
	}
}

// kate: tab-width 4
