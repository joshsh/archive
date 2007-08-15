package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Not extends PrimitiveFunction
{
	private static String tfNs = "http://fortytwo.net/2007/03/ripple/joy#";

	private RippleValue truePrim = null;
	private RippleValue falsePrim = null;

	public Not( RdfValue v, ModelConnection mc )
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
		RippleValue x;

		x = stack.getFirst();
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

		// Note: everything apart from joy:true is considered false.
		RippleValue result = ( x.equals( truePrim ) )
			? falsePrim
			: truePrim;

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
