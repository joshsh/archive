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

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.XMLSchema;

/**
 * A numeric (xsd:integer or xsd:double) literal value.
 */
public class NumericLiteral implements RippleValue
{
	/**
	 * Distinguishes between numeric literals of type xsd:integer and xsd:double.
	 */
	public enum NumericLiteralType { INTEGER, DOUBLE };

	private NumericLiteralType type;
	private Number number;

	private RdfValue rdfEquivalent = null;

	public NumericLiteral( final RdfValue rdf )
		throws RippleException
	{
		rdfEquivalent = rdf;
		Value v = rdf.getRdfValue();

		if ( !( v instanceof Literal ) )
		{
			throw new RippleException( "value " + v.toString() + " is not a Literal" );
		}

		URI dataType = ( (Literal) v ).getDatatype();

		if ( null == dataType )
		{
			throw new RippleException( "literal is untyped" );
		}

		else if ( dataType.equals( XMLSchema.INTEGER )
			|| dataType.equals( XMLSchema.INT ) )
		{
			try
			{
				type = NumericLiteralType.INTEGER;
				number = new Integer( ( (Literal) v ).intValue() );
			}

			catch ( Throwable t )
			{
				throw new RippleException( t );
			}
		}

		else if ( dataType.equals( XMLSchema.DOUBLE ) )
		{
			try
			{
				type = NumericLiteralType.DOUBLE;
				number = new Double( ( (Literal) v ).doubleValue() );
			}

			catch ( Throwable t )
			{
				throw new RippleException( t );
			}
		}
		else
		{
			throw new RippleException( "not a recognized numeric data type: " + dataType );
		}
	}

	public NumericLiteral( final int i )
	{
		type = NumericLiteralType.INTEGER;
		number = new Integer( i );
	}

	public NumericLiteral( final double d )
	{
		type = NumericLiteralType.DOUBLE;
		number = new Double( d );
	}

	public NumericLiteralType getType()
	{
		return type;
	}

	public int intValue()
	{
		return number.intValue();
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

	public static NumericLiteral abs( final NumericLiteral a )
	{
		if ( NumericLiteralType.INTEGER == a.type )
		{
			return new NumericLiteral( Math.abs( a.intValue() ) );
		}

		else
		{
			return new NumericLiteral( Math.abs( a.doubleValue() ) );
		}
	}

	public static NumericLiteral neg( final NumericLiteral a )
	{
		if ( NumericLiteralType.INTEGER == a.type )
		{
			return new NumericLiteral( -a.intValue() );
		}

		else
		{
			// Note: avoids negative zero.
			return new NumericLiteral( 0.0 - a.doubleValue() );
		}
	}

	public static NumericLiteral add( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
		{
			return new NumericLiteral( a.number.intValue() + b.number.intValue() );
		}

		else
		{
			return new NumericLiteral( a.number.doubleValue() + b.number.doubleValue() );
		}
	}

	public static NumericLiteral sub( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
		{
			return new NumericLiteral( a.number.intValue() - b.number.intValue() );
		}

		else
		{
			return new NumericLiteral( a.number.doubleValue() - b.number.doubleValue() );
		}
	}

	public static NumericLiteral mul( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
		{
			return new NumericLiteral( a.number.intValue() * b.number.intValue() );
		}

		else
		{
			return new NumericLiteral( a.number.doubleValue() * b.number.doubleValue() );
		}
	}

	// Note: does not check for divide-by-zero.
	public static NumericLiteral div( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
		{
			return new NumericLiteral( a.number.intValue() / b.number.intValue() );
		}

		else
		{
			return new NumericLiteral( a.number.doubleValue() / b.number.doubleValue() );
		}
	}

	// Note: does not check for divide-by-zero.
	public static NumericLiteral mod( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
		{
			return new NumericLiteral( a.intValue() % b.intValue() );
		}

		else
		{
			return new NumericLiteral( a.doubleValue() % b.doubleValue() );
		}
	}

	public static NumericLiteral pow( final NumericLiteral a,
										final NumericLiteral pow )
	{
		double r = Math.pow( a.doubleValue(), pow.doubleValue() );

		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == pow.type )
		{
			return new NumericLiteral( (int) r );
		}

		else
		{
			return new NumericLiteral( r );
		}
	}

	// RippleValue methods /////////////////////////////////////////////////////

	public RdfValue toRdf( final ModelConnection mc )
		throws RippleException
	{
		if ( null == rdfEquivalent )
		{
			switch ( type )
			{
				case INTEGER:
					rdfEquivalent = mc.createValue( number.intValue() );
					break;
				case DOUBLE:
					rdfEquivalent = mc.createValue( number.doubleValue() );
					break;
			}
		}

		return rdfEquivalent;
	}

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
			case DOUBLE:
				p.print( number.doubleValue() );
				break;
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
				return compareTo( new NumericLiteral( (RdfValue) other ) );
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
}

// kate: tab-width 4
