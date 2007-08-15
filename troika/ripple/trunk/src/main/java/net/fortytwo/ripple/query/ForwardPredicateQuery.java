/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class ForwardPredicateQuery implements Function
{
	private RdfValue pred;
	private ModelBridge bridge;

	private class ForwardPredicateQueryResultSink implements Sink<RdfValue>
	{
		private Sink<RippleList> sink;
		private RippleList stack;

		public ForwardPredicateQueryResultSink( RippleList stack, Sink<RippleList> sink )
		{
			this.stack = stack;
			this.sink = sink;
		}

		public void put( RdfValue v )
			throws RippleException
		{
			sink.put(
				new RippleList( bridge.get( v ), stack ) );
		}
	}

	public ForwardPredicateQuery( RdfValue predicate )
	{
		pred = predicate;
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		bridge = mc.getModel().getBridge();

		RippleValue first = stack.getFirst();
		RippleList rest = stack.getRest();

		Sink<RdfValue> querySink = new ForwardPredicateQueryResultSink( rest, sink );

//		mc.multiply( first.toRdf( mc ), pred, querySink );
		mc.multiplyAsynch( first.toRdf( mc ), pred, querySink );
	}
}

// kate: tab-width 4
