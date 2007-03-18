package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerGreaterThan extends PrimitiveFunction
{
	public IntegerGreaterThan( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 2;
	}

	protected void applyInternal( RippleStack stack,
								Sink<RippleStack> sink,
								ModelConnection mc )
		throws RippleException
	{
		int a, b;
		Combinator result;

		a = mc.intValue(
			mc.castToLiteral( stack.getFirst() ) );
		stack = stack.getRest();
		b = mc.intValue(
			mc.castToLiteral( stack.getFirst() ) );
		stack = stack.getRest();

// TODO: we shouldn't need to create a new Combinator instance for each result.
		result = ( a > b )
			? new True( mc )
			: new False( mc );

		sink.put( new RippleStack( result, stack ) );
	}
}

// kate: tab-width 4
