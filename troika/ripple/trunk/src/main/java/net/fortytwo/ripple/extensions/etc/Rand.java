package net.fortytwo.ripple.extensions.etc;

import java.util.Random;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Rand extends PrimitiveFunction
{
	private Random random = new Random();

	public Rand()
		throws RippleException
	{
		super();
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
		int lim = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		RippleValue result = mc.createValue( random.nextInt( lim ) );

		sink.put( new RippleList( result, stack ) );
	}
}

// kate: tab-width 4
