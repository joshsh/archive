package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Prog extends PrimitiveFunction
{
	public Prog( RdfValue v, ModelConnection mc )
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

		// If not (already) a list...
		if ( !( p instanceof RippleList ) )
		{
			// If the argument is an RDF value, try to convert it to a native list.
			if ( p instanceof RdfValue )
				p = new RippleList( (RdfValue) p, mc );
			else
				throw new RippleException( "argument to rpl-new:prim is not a primitive function" );
		}

		sink.put( new RippleList(
			new Operator( (RippleList) p ), stack ) );
	}
}

// kate: tab-width 4
