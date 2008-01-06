/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;

public class Closure implements Function
{
	private Function innerFunction;
	private RippleValue argument;
	private int cachedArity;

	public Closure( final Function innerFunction, final RippleValue argument )
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
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		innerFunction.applyTo( mc.list( argument, stack ), sink,  mc );
	}
	
	public boolean isTransparent()
	{
		return innerFunction.isTransparent();
	}
	
	public String toString()
	{
		return "Closure(" + innerFunction + ", " + argument + ")";
	}
}

// kate: tab-width 4
