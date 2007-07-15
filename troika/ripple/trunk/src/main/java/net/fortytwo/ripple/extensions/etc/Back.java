package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Back extends PrimitiveFunction
{
	public Back()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 2;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue o, p;

		p = stack.getFirst();
		stack = stack.getRest();
		o = stack.getFirst();
		final RippleList rest = stack.getRest();

		Sink<RdfValue> divSink = new Sink<RdfValue>()
		{
			public void put( final RdfValue v )
				throws RippleException
			{
				sink.put(
					new RippleList( mc.getModel().getBridge().get( v ),
					rest ) );
			}
		};

		mc.divide( o.toRdf( mc ), p.toRdf( mc ), divSink );
	}
}

// kate: tab-width 4
