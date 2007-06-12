package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Substring extends PrimitiveFunction
{
	public Substring( RdfValue v, ModelConnection mc )
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
		int begin, end;
		String s, result;

		end = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		begin = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			result = s.substring( begin, end );
			sink.put( new RippleList( mc.createValue( result ), stack ) );
		}

		catch ( IndexOutOfBoundsException e )
		{
			// Silent fail.
			return;
		}
	}
}

// kate: tab-width 4
