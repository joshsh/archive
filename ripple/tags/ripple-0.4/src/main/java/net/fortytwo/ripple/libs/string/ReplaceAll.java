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

/**
 * A primitive which consumes a string, a regular expression and a replacement
 * substring, then produces the string obtained by replacing all occurrences of the
 * regular expression in the original string with the replacement substring.
 */
public class ReplaceAll extends PrimitiveFunction
{
	private static final int ARITY = 3;

	public ReplaceAll()
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
		String regex, replacement, s, result;

		replacement = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		regex = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			result = s.replaceAll( regex, replacement );
			sink.put( new RippleList( mc.createValue( result ), stack ) );
		}

		catch ( java.util.regex.PatternSyntaxException e )
		{
			// Hard fail (for now).
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
