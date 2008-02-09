package net.fortytwo.ripple.model;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.Ripple;

/**
 * Author: josh
 * Date: Feb 7, 2008
 * Time: 10:29:26 AM
 */
public class RdfPredicateRelation implements StackRelation
{
	private RippleValue predicate;
	private boolean includeInferred;
	private RippleValue inContext;

	public RdfPredicateRelation( final RippleValue pred, final boolean includeInferred )
	{
		predicate = pred;
		// FIXME
		this.includeInferred = includeInferred;
		inContext = null;
	}

	public int arity()
	{
		return 1;
	}

	public boolean isTransparent()
	{
		return true;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	) throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		RippleValue subject = stack.getFirst();
		
		Sink<RippleValue> resultSink = new ValueSink( arg, sink );

		if ( Ripple.asynchronousQueries() )
		{
			mc.multiplyAsynch( subject, predicate, resultSink, includeInferred );
		}

		else
		{
			mc.multiply( subject, predicate, resultSink, includeInferred );
		}
	}

	public String toString()
	{
		return "Predicate(" + predicate + ")";
	}

	private class ValueSink implements Sink<RippleValue>
	{
		private Sink<StackContext> sink;
		private StackContext arg;

		public ValueSink( final StackContext arg, final Sink<StackContext> sink )
		{
			this.arg = arg;
			this.sink = sink;
		}

		public void put( final RippleValue v ) throws RippleException
		{
			sink.put( arg.with( arg.getStack().getRest().push( v ) ) );
		}
	}
}
