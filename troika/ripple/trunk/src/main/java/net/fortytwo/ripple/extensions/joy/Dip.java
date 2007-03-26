package net.fortytwo.ripple.extensions.joy;

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
		RippleValue x, y;

// hack...
		{
			x = stack.getFirst();
			stack = stack.getRest();
			y = stack.getFirst();
			stack = stack.getRest();

			Operator o = Operator.createOperator( y, mc );
			sink.put( new RippleList( o, stack ).push( x ) );
		}
	}
}

// kate: tab-width 4
