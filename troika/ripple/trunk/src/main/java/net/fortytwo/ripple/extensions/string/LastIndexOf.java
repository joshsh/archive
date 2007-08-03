package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class LastIndexOf extends PrimitiveFunction
{
	public LastIndexOf()
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
		String str, substr;
		int result;

		substr = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		str = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = str.lastIndexOf( substr );
		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}
}

// kate: tab-width 4
