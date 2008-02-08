/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/libs/stream/Unique.java $
 * $Revision: 994 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.libs.stack.StackLibrary;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.ListMemoizer;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A filter which drops any stack which has already been transmitted and behaves
 * like the identity filter otherwise, making a stream of stacks into a set of
 * stacks.
 */
public class Intersect extends PrimitiveFunction
{
	private static final int ARITY = 2;
	private static final String MEMO = "memo";

	public Intersect() throws RippleException
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
		RippleValue rtrue = stack.getFirst();
		stack = stack.getRest();
		RippleValue rfalse = stack.getFirst();
		stack = stack.getRest();

		Operator inner = new Operator( new IntersectInner() );
		sink.put( stack.push( rtrue ).push( Operator.OP ).push( StackLibrary.getTrueValue() ).push( inner ) );
		sink.put( stack.push( rfalse ).push( Operator.OP ).push( StackLibrary.getFalseValue() ).push( inner ) );
	}

	////////////////////////////////////////////////////////////////////////////

	protected class IntersectInner implements Function
	{
		private ListMemoizer<RippleValue, String> trueMemoizer = null;
		private ListMemoizer<RippleValue, String> falseMemoizer = null;

		public int arity()
		{
			// Require that the remainder of the stack (below the marker) is
			// in normal form.  This is somewhat arbitrary.
			return 2;
		}

		public void applyTo( RippleList stack,
							final Sink<RippleList> sink,
							final Context context )
			throws RippleException
		{
			RippleValue marker = stack.getFirst();

			if ( marker.equals( StackLibrary.getTrueValue() ) )
			{
				applyTrue( stack.getRest(), sink, context );
			}

			if ( marker.equals( StackLibrary.getFalseValue() ) )
			{
				applyFalse( stack.getRest(), sink, context );
			}
		}

		private void applyTrue( final RippleList stack,
								final Sink<RippleList> sink,
								final Context context ) throws RippleException
		{
			if ( null == trueMemoizer )
			{
				trueMemoizer = new ListMemoizer<RippleValue, String>( stack, MEMO );
			}

			else
			{
				trueMemoizer.put( stack, MEMO );
			}

			if ( null != falseMemoizer && null != falseMemoizer.get( stack ) )
			{
				sink.put( stack );
			}
		}

		private void applyFalse( final RippleList stack,
								final Sink<RippleList> sink,
								final Context context ) throws RippleException
		{
			if ( null == falseMemoizer )
			{
				falseMemoizer = new ListMemoizer<RippleValue, String>( stack, MEMO );
			}

			else
			{
				falseMemoizer.put( stack, MEMO );
			}

			if ( null != trueMemoizer && null != trueMemoizer.get( stack ) )
			{
				sink.put( stack );
			}
		}

		public boolean isTransparent()
		{
			return true;
		}
	}
}