package net.fortytwo.ripple.extensions.intmath;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerLessThan extends PrimitiveFunction
{
	private static String tfNs = "http://fortytwo.net/2007/03/ripple/joy#";

	private RippleValue truePrim = null;
	private RippleValue falsePrim = null;
	
	public IntegerLessThan( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
		this.truePrim = truePrim;
		this.falsePrim = falsePrim;
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
		int a, b;
		RippleValue result;

		b = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		a = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		if ( null == truePrim )
		{
			truePrim = mc.getModel().getBridge().get(
				new RdfValue( mc.createUri( tfNs + "true" ) ) );
			falsePrim = mc.getModel().getBridge().get(
				new RdfValue( mc.createUri( tfNs + "false" ) ) );
			if ( null == truePrim || null == falsePrim )
				throw new RippleException( "boolean primitives not found" );
		}

		result = ( a < b )
			? truePrim
			: falsePrim;

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
