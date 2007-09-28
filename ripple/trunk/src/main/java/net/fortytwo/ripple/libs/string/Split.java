/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

public class Split extends PrimitiveFunction
{
	private static final int ARITY = 2;

	public Split()
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
		String s, regex;

		regex = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			String [] array = s.split( regex );
			RippleList result = RippleList.NIL;
			for ( int i = 0; i < array.length; i++ )
			{
				result = new RippleList( mc.createValue( array[i] ), result );
			}

			sink.put( new RippleList( result, stack ) );
		}

		catch ( java.util.regex.PatternSyntaxException e )
		{
			// Hard fail (for now).
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
