/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Stack extends PrimitiveFunction
{
	public Stack()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 0;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
// TODO: this should be even simpler.
		if ( RippleList.NIL != stack )
			sink.put( new RippleList( RippleList.invert( stack ), stack ) );
	}
}

// kate: tab-width 4
