package net.fortytwo.ripple.extensions.intmath;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Sign extends PrimitiveFunction
{
	public Sign( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
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

		sink.put( new RippleList(
			mc.createValue( a == 0 ? 0 : a > 0 ? 1 : -1 ),
			stack ) );
	}
}

// kate: tab-width 4
