package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.filter.Filter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Limit extends PrimitiveFunction
{
	public Limit( RdfValue v, ModelConnection mc )
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
		int lim;

		lim = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		sink.put(
			new RippleList(
				new Operator(
					new LimitInner( (long) lim ) ), stack ) );
	}

	////////////////////////////////////////////////////////////////////////////

	protected class LimitInner extends Filter
	{
		long count, limit;
	
		public LimitInner( long lim )
		{
			limit = lim;
			count = 0;
	//System.out.println( "" + this + "()" );
		}
	
		public void applyTo( RippleList stack,
							Sink<RippleList> sink,
							ModelConnection mc )
			throws RippleException
		{
			if ( count < limit )
			{
				count++;
				sink.put( stack );
			}
		}
	}
}

// kate: tab-width 4
