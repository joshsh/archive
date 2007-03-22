package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Equal extends PrimitiveFunction
{
	private True truePrim;
	private False falsePrim;

	public Equal( RdfValue v, ModelConnection mc, True truePrim, False falsePrim )
		throws RippleException
	{
		super( v, mc );

		this.truePrim = truePrim;
		this.falsePrim = falsePrim;
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
		RippleValue a, b;
		PrimitiveFunction result;

		a = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		result = ( a.equals( b ) )
			? truePrim
			: falsePrim;

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
