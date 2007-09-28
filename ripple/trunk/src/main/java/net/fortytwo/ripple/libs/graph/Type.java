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
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.Literal;
import org.openrdf.model.URI;

public class Type extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Type()
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

		v = stack.getFirst().toRdf( mc ).getRdfValue();
		stack = stack.getRest();

		if ( v instanceof Literal )
		{
			URI type = ( (Literal) v ).getDatatype();

			if ( null != type )
			{
				sink.put( new RippleList( new RdfValue( type ), stack ) );
			}
		}
	}
}

// kate: tab-width 4
