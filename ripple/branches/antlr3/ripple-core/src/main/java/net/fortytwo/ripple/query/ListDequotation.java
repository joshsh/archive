/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.StackMapping;
import net.fortytwo.ripple.flow.Sink;

public class ListDequotation implements StackMapping
{
	private RippleList list;

	public ListDequotation( final RippleList list )
	{
		this.list = list;
	}

	public int arity()
	{
		return 0;
	}

	public void applyTo( final StackContext arg,
						final Sink<StackContext, RippleException> sink )
		throws RippleException
	{
		RippleList stack = arg.getStack();

		RippleList in = list;
		RippleList out = stack;

		while ( RippleList.NIL != in )
		{
			out = out.push( in.getFirst() );
			in = in.getRest();
		}

		// Never emit an empty stack.
		if ( RippleList.NIL != out )
		{
			sink.put( arg.with( out ) );
		}
	}
	
	public boolean isTransparent()
	{
		return true;
	}
}

// kate: tab-width 4
