package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.extensions.stack.StackExtension;

public class Equal extends PrimitiveFunction
{
	public Equal()
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
		RippleValue a, b, result;

		a = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		// Note: equals() is not suitable for this operation (for instance,
		//       it may yield false for RdfValues containing identical
		//       Literals).
		result = ( 0 == a.compareTo( b ) )
			? StackExtension.getTrueValue()
			: StackExtension.getFalseValue();

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
