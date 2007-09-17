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

import org.openrdf.model.vocabulary.RDF;

public class Operator implements RippleValue
{
	public static final Operator
		OP = new Operator( new Op() );

	static final RdfValue RDF_FIRST = new RdfValue( RDF.FIRST );
	static final RdfValue RDF_NIL = new RdfValue( RDF.NIL );

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

	/**
	 *  Finds the type of a value and creates an appropriate "active" wrapper.
	 */
	protected static Operator createOperator( final RippleValue v, final ModelConnection mc )
		throws RippleException
	{
		// A function becomes active.
		if ( v instanceof Function )
		{
			return new Operator( (Function) v );
		}

		// A list is dequoted.
		else if ( v instanceof RippleList )
		{
			return new Operator( (RippleList) v );
		}

		// This is the messy part.  Attempt to guess the type of the object from
		// the available RDF statements, and create the appropriate object.
		if ( v instanceof RdfValue )
		{
			if ( isRdfList( (RdfValue) v, mc ) )
			{
				return new Operator( RippleList.createList( (RdfValue) v, mc ) );
			}

			// An RDF value not otherwise recognizable becomes a predicate filter.
			else
			{
				return new Operator( (RdfValue) v );
			}
		}

		// Anything else becomes an active nullary filter with no output.
		else
		{
			return new Operator( new NullFilter() );
		}
	}

	public boolean isActive()
	{
		return true;
	}

// TODO: replace this with something a little more clever
	private static boolean isRdfList( final RdfValue v, final ModelConnection mc )
		throws RippleException
	{
		return ( v.equals( RDF_NIL )
			|| null != mc.findSingleObject( (RdfValue) v, RDF_FIRST ) );
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
