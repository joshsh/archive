package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Ifte extends PrimitiveFunction
{
	private True truePrim;
	private False falsePrim;

	public Ifte( RdfValue v, ModelConnection mc, True truePrim, False falsePrim )
		throws RippleException
	{
		super( v, mc );
		this.truePrim = truePrim;
		this.falsePrim = falsePrim;
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

		RippleValue result;
		if ( b.equals( truePrim ) )
			result = ifPart;
		else if ( b.equals( falsePrim ) )
			result = elsePart;
		else
			throw new RippleException( "ifte expects one of the values true, false as its third argument" );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
