/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.query.ListDequotation;

public class Operator implements RippleValue
{
	public static final Operator
		OP = new Operator( new Op() );

	private Function func;
	private RdfValue rdfEquivalent = null;

	public Operator( final RdfValue pred )
	{
		func = new RdfPredicateFunction( pred );
	}

	public Operator( final Function function )
	{
//System.out.println( "Operator[" + this + "](" + function + ")" );
		func = function;
	}

	public Operator( final RippleList list )
	{
//System.out.println( "Operator[" + this + "](" + list + ")" );
		func = new ListDequotation( list );
	}

	public Function getFunction()
	{
		return func;
	}

	public String toString()
	{
		return "OP[" + func + "]";
	}

	public void printTo( final RipplePrintStream p )
		throws RippleException
	{
		p.print( rdfEquivalent );
	}

	public boolean isActive()
	{
		return true;
	}

public RdfValue toRdf( final ModelConnection mc )
	throws RippleException
{
// Note: only correct for OP, but I expect this method only to be used with OP anyway
if ( null == rdfEquivalent )
{
	rdfEquivalent = new RdfValue( mc.createUri( "http://fortytwo.net/2007/03/ripple/schema#op" ) );
}
return rdfEquivalent;
}

	public int compareTo( final RippleValue other )
	{
		if ( other instanceof Operator )
		{
			// For now, all Operators are considered equal, as the only Operator
			// which should ever be subjected to a compareTo is rpl:op.
			return 0;
		}

		else
		{
			return this.getClass().getName().compareTo( other.getClass().getName() );
		}
	}
}

// kate: tab-width 4
