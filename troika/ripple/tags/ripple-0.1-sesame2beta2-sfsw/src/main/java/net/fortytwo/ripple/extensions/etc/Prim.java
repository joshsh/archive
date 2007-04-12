package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Prim extends PrimitiveFunction
{
	public Prim( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue p;

		p = stack.getFirst();
		stack = stack.getRest();

		// Note: as far as Operator is concerned, the argument need only be a
		//       Function, as opposed to a PrimitiveFunction in particular.
		if ( !( p instanceof PrimitiveFunction ) )
			throw new RippleException( "argument to prim is not a primitive function" );

		sink.put( new RippleList(
			new Operator( (Function) p ), stack ) );
	}
}

// kate: tab-width 4
