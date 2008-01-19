/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.*;
import net.fortytwo.ripple.util.Sink;

// kate: tab-width 4

/**
 * A primitive which optionally activates ("applies") the topmost item on the
 * stack.
 */
public class IStar extends PrimitiveFunction
{
	// TODO: arity should really be 1, but this is a nice temporary solution
	private static final int ARITY = 2;

	public IStar() throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue first = stack.getFirst();

		sink.put( stack.getRest() );

		/*Sink<Operator> opSink = new Sink<Operator>()
		{
			public void put( final Operator op ) throws RippleException
			{
				
			}
		};*/

// hack...
		sink.put( stack
				.push( Operator.OP )
				.push( first )
				.push( new Operator( StackLibrary.getIstarValue() ) ) );
	}

	/*
	private class StarClosure implements Function
	{
		private Function innerFunction;

		public StarClosure( final Function f )
		{
			innerFunction = f;
		}

		public int arity()
		{
			// TODO
			return 1;
		}

		public boolean isTransparent()
		{
			return innerFunction.isTransparent();
		}

		public void applyTo( final RippleList stack,
							 final Sink<RippleList> sink,
							 final ModelConnection mc ) throws RippleException
		{
			innerFunction.applyTo( stack, sink, mc );
		}
	}*/
}