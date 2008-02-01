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

/**
 * A primitive which consumes an item and a number n, then pushes n active
 * copies of the item to the stack.  This has the effect of applying the
 * filter "n times" to the remainder of the stack.
 */
public class Times extends PrimitiveFunction
{
	// TODO: arity should really be 1
	private static final int ARITY = 2;

	public Times()
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
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		final int times;

		times = mc.toNumericValue( stack.getFirst() ).intValue();
		stack = stack.getRest();
		RippleValue p = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<Operator> opSink = new Sink<Operator>()
		{
			public void put( final Operator op ) throws RippleException
			{
				sink.put( rest.push( new Operator(
						new TimesFunction( op.getFunction(), times, false ) ) ) );
			}
		};

		Operator.createOperator( p, opSink, mc );


		/*
		if ( times < 0 )
		{
			throw new RippleException(
				"first argument to Times must be a positive integer" );
		}

		for ( int i = 0; i < times; i++ )
		{
			stack = mc.list( p, stack ).push( Operator.OP );
		}

		sink.put( stack );*/
	}


}

// kate: tab-width 4
