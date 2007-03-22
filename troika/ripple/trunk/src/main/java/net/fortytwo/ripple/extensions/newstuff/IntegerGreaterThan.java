package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerGreaterThan extends PrimitiveFunction
{
	private True truePrim;
	private False falsePrim;
	
	public IntegerGreaterThan( RdfValue v, ModelConnection mc, True truePrim, False falsePrim )
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
		int a, b;
		PrimitiveFunction result;

		a = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		b = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

// TODO: we shouldn't need to create a new Combinator instance for each result.
		result = ( a > b )
			? truePrim
			: falsePrim;

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
