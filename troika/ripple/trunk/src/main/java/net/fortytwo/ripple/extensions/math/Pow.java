package net.fortytwo.ripple.extensions.math;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Pow extends PrimitiveFunction
{
	public Pow()
		throws RippleException
	{
		super();
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
		int p, x, result;

		p = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		x = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		int r;

		if ( p < 0 )
			r = 0;

		else
		{
			r = 1;

			for ( int i = 0; i < p; i++ )
				r *= x;
		}
		
		sink.put( new RippleList( mc.createValue( r ), stack ) );
	}
}

// kate: tab-width 4
