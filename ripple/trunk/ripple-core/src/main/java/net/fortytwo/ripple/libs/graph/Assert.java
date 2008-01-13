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
 * A primitive which consumes a subject, predicate and object, then produces the
 * subject after adding the corresponding RDF statement to the triple store.
 */
public class Assert extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public Assert()
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
		RippleValue subj, pred, obj;

		obj = stack.getFirst();
		stack = stack.getRest();
		pred = stack.getFirst();
		stack = stack.getRest();
		subj = stack.getFirst();
		stack = stack.getRest();

//System.out.println( "Asserting (" + subj + ", " + pred + ", " + obj + ")" );
		mc.add( subj, pred, obj );
//System.out.println( "    done." );

		sink.put( mc.list( subj, stack ) );
	}
}

// kate: tab-width 4
