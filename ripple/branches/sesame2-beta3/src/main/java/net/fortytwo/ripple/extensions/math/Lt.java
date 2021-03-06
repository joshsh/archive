package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.extensions.stack.StackExtension;

public class Lt extends PrimitiveFunction
{
	public Lt( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
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
		int a, b;
		RippleValue result;

		b = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		a = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		result = ( a < b )
			? StackExtension.getTrueValue()
			: StackExtension.getFalseValue();

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
