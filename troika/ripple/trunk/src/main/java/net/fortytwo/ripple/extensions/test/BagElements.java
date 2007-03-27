package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.util.Iterator;

public class BagElements extends PrimitiveFunction
{
	public BagElements( RdfValue v, ModelConnection mc )
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
		Iterator<RippleValue> values;

		values = mc.bagValue( stack.getFirst() ).iterator();
		stack = stack.getRest();

		while ( values.hasNext() )
			sink.put( new RippleList( values.next(), stack ) );
	}
}

// kate: tab-width 4
