package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Rotated extends PrimitiveFunction
{
	public Rotated()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 4;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue w, z, y, x;

		w = stack.getFirst();
		stack = stack.getRest();
		z = stack.getFirst();
		stack = stack.getRest();
		y = stack.getFirst();
		stack = stack.getRest();
		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( z, stack ).push( y ).push( x ).push( w ) );
	}
}

// kate: tab-width 4
