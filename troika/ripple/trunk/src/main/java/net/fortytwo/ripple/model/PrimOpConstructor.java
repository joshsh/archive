package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.filter.Filter;
import net.fortytwo.ripple.util.Sink;

public class PrimOpConstructor extends Filter
{
	public void applyTo( RippleList stack,
				Sink<RippleList> sink,
				ModelConnection mc )
		throws RippleException
	{
		RippleValue v;
	
		v = stack.getFirst();
		stack = stack.getRest();
	
		// Note: we've only required the argument to be a Function, as opposed
		//       to a PrimitiveFunction.
		Function f;
		if ( v instanceof Function )
			f = (Function) v;
		else
			throw new RippleException( "prim expects a function as its argument" );

		sink.put( new RippleList( new Operator( f ), stack ) );
	}
}

// kate: tab-width 4
