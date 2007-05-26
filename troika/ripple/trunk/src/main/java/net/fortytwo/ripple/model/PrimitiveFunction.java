package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;

public abstract class PrimitiveFunction implements Function, RippleValue
{
	private RdfValue rdfEquivalent;
	private FunctionTypeAnnotation typeAnnotation;

	public PrimitiveFunction( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		rdfEquivalent = v;

		typeAnnotation = new FunctionTypeAnnotation( v, mc );
	}

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		p.print( rdfEquivalent );
	}

	public RdfValue toRdf( ModelConnection mc )
		throws RippleException
	{
		return rdfEquivalent;
	}

	public String toString()
	{
		return rdfEquivalent.toString();
	}

	public boolean isActive()
	{
		return false;
	}

	public int compareTo( RippleValue other )
	{
//System.out.println( "[" + this + "].compareTo(" + other + ")" );
		if ( other instanceof PrimitiveFunction )
		{
			if ( other == this )
				return 0;
			else if ( this.hashCode() < other.hashCode() )
				return -1;
			else
				return 1;
		}

		else
			return this.getClass().getName().compareTo( other.getClass().getName() );
	}
}

// kate: tab-width 4
