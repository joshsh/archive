/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/src/main/java/net/fortytwo/ripple/libs/string/PercentEncode.java $
 * $Revision: 890 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

/**
 * A primitive which consumes an  (RFC 3986) percent-encoded string and produces
 * its decoded equivalent.
 */
public class PercentDecode extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public PercentDecode()
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
		String a, result;

		a = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = StringUtils.percentDecode( a );
		sink.put( mc.list( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
