package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class I extends PrimitiveFunction
{
	public I( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
// Arguably 0...
		return 1;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
//		RippleValue v;

// hack...
		{
			sink.put( new RippleList( Operator.OP, stack ) );
		}
/*
		RippleList l;

		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();

		sink.put( pushReversed( l, stack ) );
*/
	}

/*
	private RippleList pushReversed( RippleList in, RippleList out )
	{
		return ( null == in )
			? out
			: new RippleList( in.getFirst(), pushReversed( in.getRest(), out ) );
	}
*/
}

// kate: tab-width 4
