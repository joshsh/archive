package net.fortytwo.ripple.model;

import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.flow.Mapping;
import net.fortytwo.ripple.flow.NullMapping;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.Ripple;

/**
 * Author: josh
 * Date: Feb 7, 2008
 * Time: 10:29:26 AM
 */
public class RdfPredicateMapping implements StackMapping
{
	private RippleValue predicate;
	private boolean includeInferred;
	private RippleValue inContext;

	public RdfPredicateMapping( final RippleValue pred, final boolean includeInferred )
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
						 final Sink<StackContext, RippleException> sink ) throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		RippleValue subject = stack.getFirst();
		
		Sink<RippleValue, RippleException> resultSink = new ValueSink( arg, sink );

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

    // TODO: an RDF predicate mapping has a well-defined inverse mapping
    public StackMapping inverse() throws RippleException
    {
        return new NullStackMapping();
    }

    private class ValueSink implements Sink<RippleValue, RippleException>
	{
		private Sink<StackContext, RippleException> sink;
		private StackContext arg;

		public ValueSink( final StackContext arg, final Sink<StackContext, RippleException> sink )
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
