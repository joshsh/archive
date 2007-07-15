package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class ReplaceAll extends PrimitiveFunction
{
	public ReplaceAll()
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
		String regex, replacement, s, result;

		replacement = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		regex = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			result = s.replaceAll( regex, replacement );
			sink.put( new RippleList( mc.createValue( result ), stack ) );
		}

		catch ( java.util.regex.PatternSyntaxException e )
		{
			// Hard fail (for now).
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
