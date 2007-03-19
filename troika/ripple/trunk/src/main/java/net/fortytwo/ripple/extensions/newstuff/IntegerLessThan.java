package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerLessThan extends PrimitiveFunction
{
	// TODO: this is slightly awkward
	private True truePrim;
	private False falsePrim;
	
	public IntegerLessThan( RdfValue v, ModelConnection mc, True truePrim, False falsePrim )
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

	public void applyTo( RippleStack stack,
								Sink<RippleStack> sink,
								ModelConnection mc )
		throws RippleException
	{
		int a, b;
		PrimitiveFunction result;

		a = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		b = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		result = ( a < b )
			? truePrim
			: falsePrim;

		sink.put( new RippleStack( result, stack ) );
	}
}

// kate: tab-width 4
