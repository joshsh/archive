package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Limit extends PrimitiveFunction
{
	public Limit( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	public void applyInternal( RippleStack stack,
						Sink<RippleStack> sink,
						ModelConnection mc )
		throws RippleException
	{
		int lim;

		lim = mc.intValue(
			mc.castToLiteral( stack.getFirst() ) );
		stack = stack.getRest();

		sink.put(
			new RippleStack(
				FunctionEnvelope.createEnvelope(
					new net.fortytwo.ripple.model.filter.Limit( (long) lim ) ), stack ) );
	}
}

// kate: tab-width 4
