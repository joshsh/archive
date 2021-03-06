package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class At extends PrimitiveFunction
{
	public At( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
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
		int i;
		RippleList l;

		i = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		l = RippleList.invert( RippleList.from( stack.getFirst(), mc ) );
		stack = stack.getRest();

		if ( i < 1 )
			throw new RippleException( "list index out of bounds (keep in mind that 'at' begins counting at 1): " + i );

		for ( int j = 1; j < i; j++ )
		{
			l = l.getRest();
			if ( null == l )
				throw new RippleException( "list index out of bounds: " + i );
		}

		sink.put( new RippleList( l.getFirst(), stack ) );
	}
}

// kate: tab-width 4
