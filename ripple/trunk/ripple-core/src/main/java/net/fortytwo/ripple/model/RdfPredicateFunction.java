/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class RdfPredicateFunction implements Function
{
	private RdfValue pred;
	private boolean includeInferred;

	private class ValueSink implements Sink<RippleValue>
	{
		private Sink<RippleList> sink;
		private RippleList stack;

		public ValueSink( final RippleList stack, final Sink<RippleList> sink )
		{
			this.stack = stack;
			this.sink = sink;
		}

		public void put( final RippleValue v ) throws RippleException
		{
			sink.put( stack.push( v ) );
		}
	}

	public RdfPredicateFunction( final RdfValue predicate, final boolean includeInferred )
	{
		pred = predicate;
		this.includeInferred = includeInferred;
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue first = stack.getFirst();
		stack = stack.getRest();

		Sink<RippleValue> querySink = new ValueSink( stack, sink );

		if ( Ripple.asynchronousQueries() )
		{
			mc.multiplyAsynch( first, pred, querySink, includeInferred );		
		}
		
		else
		{
			mc.multiply( first.toRdf( mc ), pred, querySink, includeInferred );
		}
	}
	
	public boolean isTransparent()
	{
		return true;
	}
	
	public String toString()
	{
		return "Predicate(" + pred + ")";
	}
}

// kate: tab-width 4
