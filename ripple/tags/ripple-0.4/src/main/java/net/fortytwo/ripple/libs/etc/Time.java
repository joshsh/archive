/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.etc;

import java.util.Date;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which produces the current time, in seconds since midnight UTC of
 * January 1, 1970.
 */
public class Time extends PrimitiveFunction
{
	private static final int ARITY = 0;
	private static final int ONE_THOUSAND = 1000;

	public Time()
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
		sink.put( new RippleList(
			mc.createValue( (int) ( ( new Date() ).getTime() / ONE_THOUSAND ) ),
			stack ) );
	}
}

// kate: tab-width 4
