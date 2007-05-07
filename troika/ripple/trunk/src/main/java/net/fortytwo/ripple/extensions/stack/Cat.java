package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Cat extends PrimitiveFunction
{
	public Cat( RdfValue v, ModelConnection mc )
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
		RippleList l1, l2;

		l1 = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();
		l2 = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		RippleList result = RippleList.concat( l1, l2 );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
