package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Cons extends PrimitiveFunction
{
	public Cons( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 2;
	}

	private RippleList bury( RippleValue last, RippleList l )
	{
		if ( RippleList.NIL == l )
			return new RippleList( last );
		else
			return new RippleList( l.getFirst(), bury( last, l.getRest() ) );
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue x;
		RippleList l;

		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();
		x = stack.getFirst();
		stack = stack.getRest();

		sink.put( new RippleList( bury( x, l ), stack ) );
	}
}

// kate: tab-width 4
