/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a numeric "arity" and produces an active identity
 * filter with the given arity.  This forces the remainder of the stack to be
 * reduced to the corresponding depth.
 */
public class Ary extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Ary()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	private class NaryId implements Function
	{
		private int n;

		public NaryId( final int arity )
		{
			n = arity;
		}

		public int arity()
		{
			return n;
		}

		public void applyTo( RippleList stack,
							final Sink<RippleList> sink,
							final ModelConnection mc )
			throws RippleException
		{
			sink.put( stack );
		}
		
		public boolean isTransparent()
		{
			return true;
		}
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		int n;

		n = mc.toNumericValue( stack.getFirst() ).intValue();
		stack = stack.getRest();

		sink.put(
			mc.list(
				new Operator( new NaryId( n ) ), stack ) );
	}
}

// kate: tab-width 4
