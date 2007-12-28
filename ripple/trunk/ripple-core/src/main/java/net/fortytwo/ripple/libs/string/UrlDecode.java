/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/src/main/java/net/fortytwo/ripple/libs/string/UrlEncode.java $
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
 * A primitive which consumes an application/x-www-form-urlencoded string and
 * produces its decoded equivalent.
 */
public class UrlDecode extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public UrlDecode()
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

		a = mc.toString( stack.getFirst() );
		stack = stack.getRest();

		result = StringUtils.urlDecode( a );
		sink.put( mc.list( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
