/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.model.StackMapping;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.NullStackMapping;
import net.fortytwo.ripple.ListMemoizer;

/**
 * A filter which drops any stack which has already been transmitted and behaves
 * like the identity filter otherwise, making a stream of stacks into a set of
 * stacks.
 */
public class Unique extends PrimitiveStackMapping
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink
	)
		throws RippleException
	{
		RippleList stack = arg.getStack();

		sink.put( arg.with(
			stack.push(
				new Operator(
					new UniqueInner() ) ) ) );
	}

	////////////////////////////////////////////////////////////////////////////

	protected class UniqueInner implements StackMapping
	{
		private ListMemoizer<RippleValue, String> memoizer = null;
	
		public int arity()
		{
			return 1;
		}
	
		public void applyTo( final StackContext arg,
							 final Sink<StackContext, RippleException> sink
		)
			throws RippleException
		{
			if ( null == memoizer )
			{
				memoizer = new ListMemoizer<RippleValue, String>( arg.getStack(), MEMO );
				sink.put( arg );
			}
	
			else if ( memoizer.put( arg.getStack(), MEMO ) )
			{
				sink.put( arg );
			}
		}
		
		public boolean isTransparent()
		{
			return true;
		}
        
        public StackMapping inverse() throws RippleException
        {
            return new NullStackMapping();
        }
    }
}

// kate: tab-width 4
