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

public abstract class PrimitiveStackRelation implements StackRelation, RippleValue
{
	private RdfValue rdfEquivalent = null;
//	private FunctionTypeAnnotation typeAnnotation = null;
	private boolean transparent;

	public PrimitiveStackRelation( final boolean transparent )
	{
		this.transparent = transparent;
	}
	
	public PrimitiveStackRelation()
	{
		transparent = true;
	}

	public PrimitiveStackRelation( final RdfValue v, final ModelConnection mc )
		throws RippleException
	{
		setRdfEquivalent( v, mc );
	}

	public void setRdfEquivalent( final RdfValue v, final ModelConnection mc )
		throws RippleException
	{
		rdfEquivalent = v;

//		typeAnnotation = new FunctionTypeAnnotation( v, mc );
	}

	public void printTo( final RipplePrintStream p )
		throws RippleException
	{
		p.print( rdfEquivalent );
	}

	public RdfValue toRdf( final ModelConnection mc )
		throws RippleException
	{
		return rdfEquivalent;
	}

	public String toString()
	{
		return rdfEquivalent.toString();
	}
	
	public boolean equals( final Object o )
	{
		return ( o instanceof PrimitiveStackRelation )
			? ( o == this ) : false;
	}

	public boolean isActive()
	{
		return false;
	}

	public int compareTo( final RippleValue other )
	{
//System.out.println( "[" + this + "].compareTo(" + other + ")" );
		if ( other instanceof PrimitiveStackRelation )
		{
			if ( other == this )
			{
				return 0;
			}

			else if ( this.hashCode() < other.hashCode() )
			{
				return -1;
			}

			else
			{
				return 1;
			}
		}

		else
		{
			return this.getClass().getName().compareTo( other.getClass().getName() );
		}
	}
	
	public boolean isTransparent()
	{
		return transparent;
	}
}

// kate: tab-width 4
