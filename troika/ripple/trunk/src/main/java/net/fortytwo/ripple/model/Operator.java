package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.vocabulary.RDF;

public class Operator implements RippleValue
{
	public static Operator
		OP = new Operator( new Op() );

	Function func;
	RdfValue rdfEquivalent = null;

	public Operator( RdfValue pred )
	{
		func = new ForwardPredicateQuery( pred );
	}

	public Operator( Function function )
	{
//System.out.println( "Operator[" + this + "](" + function + ")" );
		func = function;
	}

	public Operator( RippleList list )
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

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		p.print( rdfEquivalent );
	}

	private static RdfValue
		rdfType = new RdfValue( RDF.TYPE );

	private static RdfValue rdfFirst = new RdfValue( RDF.FIRST );
	private static RdfValue rdfNil = new RdfValue( RDF.NIL );

	/**
	*  Guesses the type of a node and creates an appropriate wrapper.
	*/
	protected static Operator createOperator( RippleValue v, ModelConnection mc )
		throws RippleException
	{
		if ( v instanceof Function )
			return new Operator( (Function) v );

		else if ( v instanceof RippleList )
			return new Operator( (RippleList) v );

		// This is the messy part.  Attempt to guess the type of the object from
		// the available RDF statements, and create the appropriate object.
		if ( v instanceof RdfValue )
		{
// TODO: do some type inference instead of a dumb, single lookup.
			if ( v.equals( rdfNil ) )
				return new Operator( RippleList.NIL );
			else if ( null != mc.findSingleObject( (RdfValue) v, rdfFirst ) )
				return new Operator( RippleList.createList( (RdfValue) v, mc ) );
			else
				return new Operator( (RdfValue) v );
		}

		else
			throw new RippleException( "bad RippleValue in createOperator(): "
				+ v );
	}

	public boolean isActive()
	{
		return true;
	}

public RdfValue toRdf( ModelConnection mc )
	throws RippleException
{
// Note: only correct for OP, but I expect this method only to be used with OP anyway
if ( null == rdfEquivalent )
	rdfEquivalent = new RdfValue( mc.createUri( "http://fortytwo.net/2007/03/ripple/schema#op" ) );
return rdfEquivalent;
}

	public int compareTo( RippleValue other )
	{
//System.out.println( "[" + this + "].compareTo(" + other + ")" );
		if ( other instanceof Operator )
		{
			// For now, all Operators are considered equal, as the only Operator
			// which should ever be subjected to a compareTo is OP.
			return 0;
		}

		else
			return Operator.class.getName().compareTo( other.getClass().getName() );
	}
}

// kate: tab-width 4
