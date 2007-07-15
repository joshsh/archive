package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Fold extends PrimitiveFunction
{
	public Fold()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 3;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue f, v, l;

		f = stack.getFirst();
		stack = stack.getRest();
		v = stack.getFirst();
		stack = stack.getRest();
		l = stack.getFirst();
		stack = stack.getRest();

		RippleList lList = RippleList.invert( ( l instanceof RippleList )
			? (RippleList) l
			:  RippleList.createList( l.toRdf( mc ), mc ) );

		RippleList result = new RippleList( v, stack );

		while ( RippleList.NIL != lList )
		{
			result = result.push( lList.getFirst() )
				.push( f )
				.push( Operator.OP );
			lList = lList.getRest();
		}

		sink.put( result );
	}
}

// kate: tab-width 4
