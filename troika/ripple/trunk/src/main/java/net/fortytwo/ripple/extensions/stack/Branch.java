package net.fortytwo.ripple.extensions.stack;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Branch extends PrimitiveFunction
{
	public Branch()
		throws RippleException
	{
		super();
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
		RippleValue b, trueProg, falseProg;

		falseProg = stack.getFirst();
		stack = stack.getRest();
		trueProg = stack.getFirst();
		stack = stack.getRest();
		b = stack.getFirst();
		stack = stack.getRest();

		RippleValue result;
		if ( b.equals( StackExtension.getTrueValue() ) )
			result = trueProg;
		else if ( b.equals( StackExtension.getFalseValue() ) )
			result = falseProg;
		else
			throw new RippleException( "branch expects one of the values true, false as its third argument" );

		sink.put( new RippleList( result, stack ).push( Operator.OP ) );
	}
}

// kate: tab-width 4
