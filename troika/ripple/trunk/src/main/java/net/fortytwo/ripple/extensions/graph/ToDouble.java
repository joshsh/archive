package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

import org.openrdf.model.URI;

public class ToDouble extends PrimitiveFunction
{
	private final static Logger s_logger
		= Logger.getLogger( ToDouble.class );

	public ToDouble( RdfValue v, ModelConnection mc )
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
		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		double d;

		try
		{
			d = new Double( s ).doubleValue();
		}

		catch ( NumberFormatException e )
		{
			s_logger.debug( "bad integer value: " + s );
			return;
		}

		sink.put( new RippleList( mc.createValue( d ), stack ) );
	}
}

// kate: tab-width 4