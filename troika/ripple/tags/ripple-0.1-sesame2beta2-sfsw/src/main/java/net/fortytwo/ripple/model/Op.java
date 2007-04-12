package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.filter.Filter;
import net.fortytwo.ripple.util.Sink;

public class Op extends Filter
{
	public void applyTo( RippleList stack,
				Sink<RippleList> sink,
				ModelConnection mc )
		throws RippleException
	{
		RippleValue v;
	
		v = stack.getFirst();
		stack = stack.getRest();
	
		sink.put( new RippleList(
			Operator.createOperator( v, mc ), stack ) );
	}
}

// kate: tab-width 4
