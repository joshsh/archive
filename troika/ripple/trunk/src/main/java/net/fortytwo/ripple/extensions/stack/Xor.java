package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.extensions.stack.StackExtension;

public class Xor extends PrimitiveFunction
{
	public Xor( RdfValue v, ModelConnection mc )
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
		RippleValue x, y;

		x = stack.getFirst();
		stack = stack.getRest();
		y = stack.getFirst();
		stack = stack.getRest();

		RippleValue trueValue = StackExtension.getTrueValue();

		// Note: everything apart from joy:true is considered false.
		boolean xb = ( 0 == x.compareTo( trueValue ) );
		boolean yb = ( 0 == y.compareTo( trueValue ) );
		RippleValue result = ( ( xb && !yb ) || ( !xb && yb ) )
			? trueValue
			: StackExtension.getFalseValue();

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
