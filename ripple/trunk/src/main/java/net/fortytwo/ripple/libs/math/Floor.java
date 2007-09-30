package net.fortytwo.ripple.libs.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.NumericLiteral;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

/**
 * A primitive which consumes a number and produces the largest integer value
 * which is less than or equal to the number.  Note: the produced value has
 * a type of xsd:integer, unlike Java's Math.floor, which returns a double
 * value.
 */
public class Floor extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Floor()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		NumericLiteral a, result;

		a = mc.numericValue( stack.getFirst() );
		stack = stack.getRest();

		result = new NumericLiteral( (int) Math.floor( a.doubleValue() ) );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
