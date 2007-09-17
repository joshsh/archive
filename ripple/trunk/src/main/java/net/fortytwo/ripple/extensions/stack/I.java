/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

public class I extends PrimitiveFunction
{
// Arguably 0...
	private static final int ARITY = 1;

	public I()
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
//		RippleValue v;

// hack...
		sink.put( new RippleList( Operator.OP, stack ) );
/*
		RippleList l;

		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		sink.put( pushReversed( l, stack ) );
*/
	}

/*
	private RippleList pushReversed( RippleList in, RippleList out )
	{
		return ( null == in )
			? out
			: new RippleList( in.getFirst(), pushReversed( in.getRest(), out ) );
	}
*/
}

// kate: tab-width 4
