package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class In extends PrimitiveFunction
{
	private static String tfNs = "http://fortytwo.net/2007/03/ripple/joy#";

	private RippleValue truePrim = null;
	private RippleValue falsePrim = null;

	public In( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 2;
	}

	private boolean has( RippleList l, final RippleValue v )
		throws RippleException
	{
		while ( RippleList.NIL != l )
		{
			if ( 0 == l.getFirst().compareTo( v ) )
				return true;

			l = l.getRest();
		}

		return false;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue x;
		RippleList l;

		l = RippleList.from( stack.getFirst(), mc );
		stack = stack.getRest();
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

		sink.put( new RippleList( has( l, x ) ? truePrim : falsePrim, stack ) );
	}
}

// kate: tab-width 4
