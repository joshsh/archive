/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.impl.sesame.SesameList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes an RDF container and produces all items in the
 * container.
 */
public class Contains extends PrimitiveFunction
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

	public void applyTo( final RippleList stack,
						final Sink<RippleList> sink,
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		RippleValue head = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RippleValue> pushSink = new Sink<RippleValue>()
		{
			public void put( final RippleValue v ) throws RippleException
			{
				sink.put( new SesameList( v, rest ) );
			}
		};		
		
		mc.putContainerMembers( head, pushSink );
	}
}

// kate: tab-width 4
