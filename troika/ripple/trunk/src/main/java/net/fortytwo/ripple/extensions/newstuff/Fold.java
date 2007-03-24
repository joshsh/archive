package net.fortytwo.ripple.extensions.newstuff;

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
	public Fold( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
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

		RippleList fList = ( f instanceof RippleList )
			? (RippleList) f
			: new RippleList( f.toRdf( mc ), mc );
		RippleList lList = RippleList.invert( ( l instanceof RippleList )
			? (RippleList) l
			:  new RippleList( l.toRdf( mc ), mc ) );

		RippleList result = new RippleList( v, stack );
		Operator op = new Operator( fList );

		while ( null != lList )
		{
			result = result.push( lList.getFirst() ).push( op );
			lList = lList.getRest();
		}

		sink.put( result );
	}
}

// kate: tab-width 4
