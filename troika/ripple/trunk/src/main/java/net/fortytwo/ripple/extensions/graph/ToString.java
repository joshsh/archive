package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;

public class ToString extends PrimitiveFunction
{
	public ToString()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		RippleValue v;

		v = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( mc.createValue( v.toString() ), stack ) );
	}
}

// kate: tab-width 4
