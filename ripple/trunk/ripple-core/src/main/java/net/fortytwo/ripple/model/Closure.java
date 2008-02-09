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

public class Closure implements StackRelation
{
	private StackRelation innerRelation;
	private RippleValue argument;
	private int cachedArity;

	public Closure( final StackRelation innerRelation, final RippleValue argument )
	{
		this.innerRelation = innerRelation;
		this.argument = argument;
		cachedArity = innerRelation.arity() - 1;
//System.out.println( "" + this + ": (" + innerRelation + ", " + argument + ")" );
	}

	public int arity()
	{
		return cachedArity;
	}

	public void applyTo( final StackContext arg,
						final Sink<StackContext> sink )
		throws RippleException
	{
		innerRelation.applyTo( arg.with( arg.getStack().push( argument ) ), sink );
	}
	
	public boolean isTransparent()
	{
		return innerRelation.isTransparent();
	}
	
	public String toString()
	{
		return "Closure(" + innerRelation + ", " + argument + ")";
	}
}

// kate: tab-width 4
