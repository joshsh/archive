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
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Dupdd extends PrimitiveFunction
{
	public Dupdd()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 3;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue z, y, x;

		z = stack.getFirst();
		stack = stack.getRest();
		y = stack.getFirst();
		stack = stack.getRest();
		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( x, stack ).push( x ).push( y ).push( z ) );
	}
}

// kate: tab-width 4