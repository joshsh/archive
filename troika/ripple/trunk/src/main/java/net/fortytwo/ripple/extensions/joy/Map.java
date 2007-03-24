package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Map extends PrimitiveFunction
{
	public Map( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 2;
	}

//	private RippleList mapped
	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
/*
		RippleValue f, l;;

		f = stack.getFirst();
		stack = stack.getRest();
		l = stack.getFirst();
		stack = stack.getRest();

		RippleList fList = ( f instanceof RippleList )
			? (RippleList) f
			: new RippleList( f.toRdf( mc ), mc );
		RippleList lList = ( l instanceof RippleList )
			? (RippleList) l
			: new RippleList( l.toRdf( mc ), mc );

		
		RippleValue result;
		if ( b.equals( truePrim ) )
			result = ifPart;
		else if ( b.equals( falsePrim ) )
			result = elsePart;
		else
			throw new RippleException( "ifte expects one of the values true, false as its third argument" );

		sink.put( new RippleList( result, stack ) );
*/
	}
}

// kate: tab-width 4
