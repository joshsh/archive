package net.fortytwo.ripple.model;
import net.fortytwo.ripple.util.Sink;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

public class Closure implements Function
{
	private Function innerFunction;
	private RippleValue argument;
	private int cachedArity;

	public Closure( Function innerFunction, RippleValue argument )
	{
		this.innerFunction = innerFunction;
		this.argument = argument;
		cachedArity = innerFunction.arity() - 1;
//System.out.println( "" + this + ": (" + innerFunction + ", " + argument + ")" );
	}

	public int arity()
	{
		return cachedArity;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		innerFunction.applyTo( new RippleList( argument, stack ), sink,  mc );
	}
}

// kate: tab-width 4
