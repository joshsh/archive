package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Dipd extends PrimitiveFunction
{
	public Dipd()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 3;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue z, y, x;

		{
			z = stack.getFirst();
			stack = stack.getRest();
			y = stack.getFirst();
			stack = stack.getRest();
			x = stack.getFirst();
			stack = stack.getRest();

			sink.put( new RippleList( z, stack ).push( Operator.OP ).push( x ).push( y ) );
		}
	}
}

// kate: tab-width 4
