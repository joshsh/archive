/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.StackRelation;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveStackRelation;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a number n and produces a filter which transmits
 * at most n stacks.
 */
public class Limit extends PrimitiveStackRelation
{
	private static final int ARITY = 1;

	public Limit()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		RippleList stack = arg.getStack();
		final ModelConnection mc = arg.getModelConnection();

		int lim;

		lim = mc.toNumericValue( stack.getFirst() ).intValue();
		stack = stack.getRest();

		sink.put( arg.with(
			stack.push(
				new Operator(
					new LimitInner( (long) lim ) ) ) ) );
	}

	////////////////////////////////////////////////////////////////////////////

	protected class LimitInner implements StackRelation
	{
		private long count, limit;

		public int arity()
		{
			return 1;
		}

		public LimitInner( final long lim )
		{
			limit = lim;
			count = 0;
		}
	
		public void applyTo( final StackContext arg,
							 final Sink<StackContext> sink
		)
			throws RippleException
		{
			if ( count < limit )
			{
				count++;
				sink.put( arg );
			}
		}
		
		public boolean isTransparent()
		{
			return true;
		}
	}
}

// kate: tab-width 4
