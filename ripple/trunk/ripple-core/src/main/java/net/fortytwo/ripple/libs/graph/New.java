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

/**
 * A primitive which produces a new blank node.
 */
public class New extends PrimitiveFunction
{
	private static final int ARITY = 0;

	public New()
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
		// Note: stack may be null (and this should not be a problem).
		RippleList result = mc.list(
			new RdfValue( mc.createBNode() ), stack );
//System.out.println( "Creating a new node" );

		sink.put( result );
	}
}

// kate: tab-width 4
