package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;

public class ToUri extends PrimitiveFunction
{
	public ToUri()
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
		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		URI uri = mc.createUri( s );

		sink.put( new RippleList(
			mc.getModel().getBridge().get( new RdfValue( uri ) ), stack ) );
	}
}

// kate: tab-width 4
