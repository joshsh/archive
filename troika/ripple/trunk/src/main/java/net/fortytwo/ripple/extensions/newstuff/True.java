package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class True extends PrimitiveFunction
{
	public True( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public void applyTo( RippleStack stack,
						Sink<RippleStack> sink,
						ModelConnection mc )
		throws RippleException
	{
		Value x, y;

		x = stack.getFirst();
		stack = stack.getRest();
		y = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleStack( x, stack ) );
	}
}

// kate: tab-width 4
