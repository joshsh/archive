package net.fortytwo.ripple.extensions.joy;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Times extends PrimitiveFunction
{
	public Times( RdfValue v, ModelConnection mc )
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
		int times;
		RippleList prog;

		times = mc.intValue( stack.getFirst() );
		stack = stack.getRest();
		RippleValue p = stack.getFirst();
		stack = stack.getRest();

		prog = ( p instanceof RippleList )
			? (RippleList) p
			: new RippleList( p.toRdf( mc ), mc );

		if ( times < 0 )
			throw new RippleException( "first argument to Times must be a positive integer" );

		Operator progOp = new Operator( prog );

		for ( int i = 0; i < times; i++ )
			stack = new RippleList( progOp, stack );

		sink.put( stack );
	}
}

// kate: tab-width 4
