package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Ifte extends PrimitiveFunction
{
	private static String tfNs = "http://fortytwo.net/2007/03/ripple/joy#";

	private RippleValue truePrim = null;
	private RippleValue falsePrim = null;

	public Ifte( RdfValue v, ModelConnection mc )
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
		RippleValue b, ifPart, elsePart;

		elsePart = stack.getFirst();
		stack = stack.getRest();
		ifPart = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();
System.out.println( "elsePart = " + elsePart );
System.out.println( "ifPart = " + ifPart );
System.out.println( "b = " + b );

		if ( null == truePrim )
		{
			truePrim = mc.getModel().getBridge().get(
				new RdfValue( mc.createUri( tfNs + "true" ) ) );
			falsePrim = mc.getModel().getBridge().get(
				new RdfValue( mc.createUri( tfNs + "false" ) ) );
			if ( null == truePrim || null == falsePrim )
				throw new RippleException( "boolean primitives not found" );
		}

		RippleValue result;
		if ( b.equals( truePrim ) )
			result = ifPart;
		else if ( b.equals( falsePrim ) )
			result = elsePart;
		else
			throw new RippleException( "ifte expects one of the values true, false as its third argument" );

		sink.put( new RippleList( result, stack ).push( Operator.OP ) );
	}
}

// kate: tab-width 4
