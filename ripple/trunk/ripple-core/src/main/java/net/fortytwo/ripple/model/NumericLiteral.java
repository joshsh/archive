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
import net.fortytwo.ripple.model.impl.sesame.NumericLiteralImpl;

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.XMLSchema;

/**
 * A numeric (xsd:integer or xsd:double) literal value.
 */
public abstract class NumericLiteral implements RippleValue
{
	/**
	 * Distinguishes between numeric literals of type xsd:integer and xsd:double.
	 */
	public enum NumericLiteralType { INTEGER, LONG, DOUBLE };

	protected NumericLiteralType type;
	protected Number number;

// TODO: move into implementation
	protected RdfValue rdfEquivalent = null;

	public abstract RdfValue toRdf( final ModelConnection mc ) throws RippleException;
	
	public NumericLiteralType getType()
	{
		return type;
	}
	
	protected Number getNumber()
	{
		return number;
	}

	public int intValue()
	{
		return number.intValue();
	}

	public long longValue()
	{
		return number.longValue();
	}
	
	public double doubleValue()
	{
		return number.doubleValue();
	}

	public boolean isZero()
	{
		return ( 0.0 == number.doubleValue() );
	}

	public int sign()
	{
		double x = number.doubleValue();
		return ( x == 0.0 ? 0 : x > 0 ? 1 : -1 );
	}

	////////////////////////////////////////////////////////////////////////////

	public boolean isActive()
	{
		return false;
	}

	public void printTo( final RipplePrintStream p )
		throws RippleException
	{
		switch ( type )
		{
			case INTEGER:
				p.print( number.intValue() );
				break;
			case LONG:
				p.print( number.longValue() );
				break;
			case DOUBLE:
				p.print( number.doubleValue() );
				break;
		}
	}

// TODO: implement hashCode()
	public boolean equals( final Object other )
	{
		if ( other instanceof NumericLiteral )
		{
			return ( 0 == compareTo( (NumericLiteral) other ) );
		}
		
		else
		{
			return false;
		}
	}
	
	public int compareTo( final RippleValue other )
	{
		if ( other instanceof NumericLiteral )
		{
			double n = number.doubleValue(),
				nOther =  ((NumericLiteral) other ).number.doubleValue();
// Note: doesn't take special floating-point number entities into account:
//       floating-point infinities, negative infinities, negative zero, and NaN
			return ( n > nOther ) ? 1 : ( n < nOther ) ? -1 : 0;
		}

		else if ( other instanceof RdfValue )
		{
			try
			{
				// Note: wasty
				return compareTo( new NumericLiteralImpl( (RdfValue) other ) );
			}

			catch ( RippleException e )
			{
				// Log the error, but otherwise ignore it and call the objects equal.
				e.logError();
				return 0;
			}
		}

		else
		{
			return RippleList.class.getName().compareTo( other.getClass().getName() );
		}
	}

	public String toString()
	{
		return number.toString();
	}
		
	public abstract NumericLiteral abs();
	public abstract NumericLiteral neg();
	public abstract NumericLiteral add( final NumericLiteral b );
	public abstract NumericLiteral sub( final NumericLiteral b );
	public abstract NumericLiteral mul( final NumericLiteral b );
	public abstract NumericLiteral div( final NumericLiteral b );
	public abstract NumericLiteral mod( final NumericLiteral b );
	public abstract NumericLiteral pow( final NumericLiteral pow );
}

// kate: tab-width 4
