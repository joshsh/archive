package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.filter.Filter;
import net.fortytwo.ripple.util.Sink;

public class ProgOpConstructor extends Filter
{
	public void applyTo( RippleList stack,
				Sink<RippleList> sink,
				ModelConnection mc )
		throws RippleException
	{
		RippleValue v;
	
		v = stack.getFirst();
		stack = stack.getRest();
	
		RippleList list = ( v instanceof RippleList )
		? (RippleList) v
		: new RippleList( v.toRdf( mc ), mc );
	
		sink.put( new RippleList( new Operator( list ), stack ) );
	}
}

// kate: tab-width 4
