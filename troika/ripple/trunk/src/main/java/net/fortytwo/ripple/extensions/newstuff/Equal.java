package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Equal extends PrimitiveFunction
{
	public Equal( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
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
		Value a, b;
		Combinator result;

		a = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

// TODO: we shouldn't need to create a new Combinator instance for each result.
		result = ( a.equals( b ) )
			? new True( mc )
			: new False( mc );

		sink.put( new RippleStack( result, stack ) );
	}
}

// kate: tab-width 4
