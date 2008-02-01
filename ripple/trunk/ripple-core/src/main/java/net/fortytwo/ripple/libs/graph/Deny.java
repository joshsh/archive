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
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a subject, predicate and object, then produces the
 * subject after removing the corresponding RDF statement from the triple store.
 */
public class Deny extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public Deny()
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
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		RippleValue subj, pred, obj;

		obj = stack.getFirst();
		stack = stack.getRest();
		pred = stack.getFirst();
		stack = stack.getRest();
		subj = stack.getFirst();
		stack = stack.getRest();

		mc.remove( subj, pred, obj );

		// TODO: store added and removed statements in a buffer until the
		// ModelConnection commits.  You may not simply wait to commit,
		// as writing and then reading without first committing may result
		// in a deadlock.  The LinkedDataSail already does this sort of
		// buffering, which is why it does not deadlock w.r.t. its base
		// Sail.
		mc.commit();

		sink.put( mc.list( subj, stack ) );
	}
}

// kate: tab-width 4
