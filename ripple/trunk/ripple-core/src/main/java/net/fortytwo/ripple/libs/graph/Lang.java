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

import org.openrdf.model.Value;
import org.openrdf.model.Literal;

/**
 * A primitive which consumes a plain literal value and produces its language
 * tag (or an empty string if the literal has no language tag).
 */
public class Lang extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Lang()
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
		Value v;
		String result;

		v = stack.getFirst().toRdf( mc ).getRdfValue();
		stack = stack.getRest();

		if ( v instanceof Literal )
		{
			result = ( (Literal) v ).getLanguage();

			if ( null == result )
			{
				result = "";
			}

			sink.put( new RippleList( mc.value( result ), stack ) );
		}
	}
}

// kate: tab-width 4
