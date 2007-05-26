package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Ary extends PrimitiveFunction
{
	public Ary( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	private class NaryId implements Function
	{
		int n;

		public NaryId( final int arity )
		{
			n = arity;
		}

		public int arity()
		{
			return n;
		}

		public void applyTo( RippleList stack,
							final Sink<RippleList> sink,
							final ModelConnection mc )
			throws RippleException
		{
			sink.put( stack );
		}
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		int n;

		n = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		sink.put(
			new RippleList(
				new Operator( new NaryId( n ) ), stack ) );
	}
}

// kate: tab-width 4
