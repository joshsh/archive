/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a number n and produces a filter which transmits
 * at most n stacks.
 */
public class Limit extends PrimitiveFunction
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

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		int lim;

		lim = mc.toNumericValue( stack.getFirst() ).intValue();
		stack = stack.getRest();

		sink.put(
			mc.list(
				new Operator(
					new LimitInner( (long) lim ) ), stack ) );
	}

	////////////////////////////////////////////////////////////////////////////

	protected class LimitInner implements Function
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
	
		public void applyTo( RippleList stack,
							final Sink<RippleList> sink,
							final ModelConnection mc )
			throws RippleException
		{
			if ( count < limit )
			{
				count++;
				sink.put( stack );
			}
		}
	}
}

// kate: tab-width 4
