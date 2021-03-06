package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Dip extends PrimitiveFunction
{
	public Dip( RdfValue v, ModelConnection mc )
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
		RippleValue y, x;

// hack...
		{
			y = stack.getFirst();
			stack = stack.getRest();
			x = stack.getFirst();
			stack = stack.getRest();

			sink.put( new RippleList( y, stack ).push( Operator.OP ).push( x ) );
		}
	}
}

// kate: tab-width 4
