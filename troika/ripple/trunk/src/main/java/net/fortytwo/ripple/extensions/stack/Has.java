package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.extensions.stack.StackExtension;

public class Has extends PrimitiveFunction
{
	public Has()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 2;
	}

	private boolean has( RippleList l, final RippleValue v )
		throws RippleException
	{
		while ( RippleList.NIL != l )
		{
			if ( 0 == l.getFirst().compareTo( v ) )
				return true;

			l = l.getRest();
		}

		return false;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue x;
		RippleList l;

		x = stack.getFirst();
		stack = stack.getRest();
		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		sink.put( new RippleList( has( l, x ) ? StackExtension.getTrueValue() : StackExtension.getFalseValue(), stack ) );
	}
}

// kate: tab-width 4
