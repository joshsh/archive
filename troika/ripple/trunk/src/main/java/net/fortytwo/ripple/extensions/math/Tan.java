package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.NumericLiteral;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Tan extends PrimitiveFunction
{
	public Tan()
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
		double a;
		NumericLiteral result;

		a = mc.numericValue( stack.getFirst() ).doubleValue();
		stack = stack.getRest();

// TODO: check for undefined values
		double d = Math.tan( a );
		result = new NumericLiteral( d );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
