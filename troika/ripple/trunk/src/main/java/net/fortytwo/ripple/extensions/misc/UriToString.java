package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;

public class UriToString extends PrimitiveFunction
{
	public UriToString( RdfValue v, ModelConnection mc )
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
		URI a;
		String result;

		a = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		try
		{
			result = a.toString();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}
}

// kate: tab-width 4
