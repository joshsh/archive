package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

/**
 *  A filter which yields no results, regardless of its inputs.
 */
public class NullFilter implements Function
{
	public int arity()
	{
		return 0;
	}

	public void applyTo( final RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{}
}

// kate: tab-width 4
