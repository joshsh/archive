package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Dup extends PrimitiveFunction
{
	public Dup( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleStack stack,
						Sink<RippleStack> sink,
						ModelConnection mc )
		throws RippleException
	{
		Value x;

		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleStack( x, stack ).push( x ) );
	}
}

// kate: tab-width 4
