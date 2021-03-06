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
 * A primitive which consumes a resource and produces the resource again after
 * removing all statements in the context its description.  The next time a
 * description of the resource is needed, Ripple will request a new
 * representation.
 */
public class Forget extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Forget()
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

// FIXME: this should not subvert the ModelConnection
		mc.getModel().getSail().getDereferencer().forget( v.toRdf( mc ), mc );

		// Note: the stack itself has not been altered.
		sink.put( stack );
	}
}

// kate: tab-width 4
