/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;

/**
 * A primitive which consumes an RDF container and produces all items in the
 * container.
 */
public class Contains extends PrimitiveStackMapping
{
	private static final int ARITY = 1;

	public Contains()
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

		RippleValue head = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleValue, RippleException> pushSink = new Sink<RippleValue, RippleException>()
		{
			public void put( final RippleValue v ) throws RippleException
			{
				sink.put( arg.with( rest.push( v ) ) );
			}
		};		
		
		mc.putContainerMembers( head, pushSink );
	}
}

// kate: tab-width 4
