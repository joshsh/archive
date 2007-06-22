package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

//import java.net.URLEncoder;

public class UrlEncoding extends PrimitiveFunction
{
	public UrlEncoding( RdfValue v, ModelConnection mc )
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
		String a, result;

		a = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = StringUtils.urlEncode( a );
/*
		try
		{
			result = URLEncoder.encode( a, "UTF-8" );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
*/

		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}
}

// kate: tab-width 4
