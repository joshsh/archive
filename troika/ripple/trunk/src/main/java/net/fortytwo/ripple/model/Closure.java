package net.fortytwo.ripple.model;
import net.fortytwo.ripple.util.Sink;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

public class Closure implements Function
{
	private Function innerFunction;
	private RippleValue argument;
	private int cachedArity;

	private RdfValue rdfEquivalent = null;

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

	public boolean isOperator()
	{
		return false;
	}

// TODO: does not indicate application
	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		switch ( Ripple.getExpressionOrder() )
		{
			case DIAGRAMMATIC:
				p.print( innerFunction );
				p.print( " " );
				p.print( argument );
				break;

			case ANTIDIAGRAMMATIC:
				p.print( argument );
				p.print( " " );
				p.print( innerFunction );
				break;
		}
	}

	public RdfValue toRdf( ModelConnection mc )
		throws RippleException
	{
/*
		if ( null == rdfEquivalent )
		{
			rdfEquivalent = mc.createBNode();

			mc.add( rdfEquivalent, 
		}
*/
		return rdfEquivalent;
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
