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
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

/**
 * A primitive which consumes a literal value and produces its xsd:double
 * equivalent (if any).
 */
public class ToDouble extends PrimitiveFunction
{
	private static final int ARITY = 1;

	private static final Logger LOGGER
		= Logger.getLogger( ToDouble.class );

	public ToDouble()
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
		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		double d;

		try
		{
			d = new Double( s ).doubleValue();
		}

		catch ( NumberFormatException e )
		{
			LOGGER.debug( "bad integer value: " + s );
			return;
		}

		sink.put( new RippleList( mc.createValue( d ), stack ) );
	}
}

// kate: tab-width 4
