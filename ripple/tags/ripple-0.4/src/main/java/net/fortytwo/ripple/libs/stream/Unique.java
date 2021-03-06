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
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.ListMemoizer;
import net.fortytwo.ripple.util.Sink;

/**
 * A filter which drops any stack which has already been transmitted and behaves
 * like the identity filter otherwise, making a stream of stacks into a set of
 * stacks.
 */
public class Unique extends PrimitiveFunction
{
	private static final int ARITY = 0;

	private static final String MEMO = "memo";

	public Unique()
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
		sink.put(
			new RippleList(
				new Operator(
					new UniqueInner() ), stack ) );
	}

	////////////////////////////////////////////////////////////////////////////

	protected class UniqueInner implements Function
	{
		private ListMemoizer<RippleValue, String> memoizer = null;
	
		public int arity()
		{
			return 1;
		}
	
		public void applyTo( RippleList stack,
							final Sink<RippleList> sink,
							final ModelConnection mc )
			throws RippleException
		{
			if ( null == memoizer )
			{
				memoizer = new ListMemoizer<RippleValue, String>( stack, MEMO );
				sink.put( stack );
			}
	
			else if ( memoizer.add( stack, MEMO ) )
			{
				sink.put( stack );
			}
		}
	}
}

// kate: tab-width 4
