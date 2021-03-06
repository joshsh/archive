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
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which follows consumes an object and predicate, producing
 * all subjects such that there is a backlink from the object to the subject
 * via the predicate.  Note: the backward traversal of links is much more
 * dependent on the history of query evaluation than forward traversal, which is
 * built into Ripple's query model.
 */
public class Back extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Back()
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
		RippleValue o, p;

		p = stack.getFirst();
		stack = stack.getRest();
		o = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RdfValue> divSink = new Sink<RdfValue>()
		{
			public void put( final RdfValue v )
				throws RippleException
			{
				sink.put(
					new RippleList( mc.getModel().getBridge().get( v ),
					rest ) );
			}
		};

		mc.divide( o.toRdf( mc ), p.toRdf( mc ), divSink );
	}
}

// kate: tab-width 4
