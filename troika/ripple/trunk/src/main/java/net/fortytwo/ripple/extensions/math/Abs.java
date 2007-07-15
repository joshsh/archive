package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Abs extends PrimitiveFunction
{
	public Abs()
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
		int a, result;

		a = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		result = ( a >= 0 ) ? a : -a;

		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}
}

// kate: tab-width 4
