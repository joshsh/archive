package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Div extends PrimitiveFunction
{
	public Div()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 2;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		int a, b, result;

		b = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		a = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		// Note: division by zero simply does not yield a result.
		if ( 0 != b )
			sink.put( new RippleList( mc.createValue( a / b ), stack ) );
	}
}

// kate: tab-width 4
