package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Split extends PrimitiveFunction
{
	public Split()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 2;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		String s, regex;

		regex = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			String [] array = s.split( regex );
			RippleList result = RippleList.NIL;
			for ( int i = 0; i < array.length; i++ )
				result = new RippleList( mc.createValue( array[i] ), result );

			sink.put( new RippleList( result, stack ) );
		}

		catch ( java.util.regex.PatternSyntaxException e )
		{
			// Hard fail (for now).
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
