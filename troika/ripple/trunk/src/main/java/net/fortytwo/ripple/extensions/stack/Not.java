package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.extensions.stack.StackExtension;

public class Not extends PrimitiveFunction
{
	public Not()
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
		RippleValue x;

		x = stack.getFirst();
		stack = stack.getRest();

		// Note: everything apart from joy:true is considered false.
		RippleValue result = ( 0 == x.compareTo( StackExtension.getTrueValue() ) )
			? StackExtension.getFalseValue()
			: StackExtension.getTrueValue();

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
