/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.util.Iterator;

public class Contains extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Contains()
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
		Iterator<RippleValue> values;

		values = mc.bagValue( stack.getFirst() ).iterator();
		stack = stack.getRest();

		while ( values.hasNext() )
		{
			sink.put( new RippleList( values.next(), stack ) );
		}
	}
}

// kate: tab-width 4
