package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.XMLSchema;

public class NumericLiteral implements RippleValue
{
	enum NumericLiteralType { INTEGER, DOUBLE };

	NumericLiteralType type;
	Number number;

	RdfValue rdfEquivalent = null;

	public NumericLiteral( final RdfValue rdf )
		throws RippleException
	{
		rdfEquivalent = rdf;
		Value v = rdf.getRdfValue();

		if ( !( v instanceof Literal ) )
			throw new RippleException( "value " + v.toString() + " is not a Literal" );

		URI dataType = ( (Literal) v ).getDatatype();

		if ( null == dataType )
			throw new RippleException( "literal is untyped" );

		else if ( dataType.equals( XMLSchema.INTEGER )
			|| dataType.equals( XMLSchema.INT ) )
		{
			try
			{
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
				number = new Double( ( (Literal) v ).doubleValue() );
			}

			catch ( Throwable t )
			{
				throw new RippleException( t );
			}
		}
		else
			throw new RippleException( "not a recognized numeric data type: " + dataType );
	}

	public NumericLiteral( int i )
	{
		type = NumericLiteralType.INTEGER;
		number = new Integer( i );
	}

	public NumericLiteral( double d )
	{
		type = NumericLiteralType.DOUBLE;
		number = new Double( d );
	}

	public static NumericLiteral add( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
			return new NumericLiteral( a.number.intValue() + b.number.intValue() );
		else
			return new NumericLiteral( a.number.doubleValue() + b.number.doubleValue() );
	}

	public static NumericLiteral sub( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
			return new NumericLiteral( a.number.intValue() - b.number.intValue() );
		else
			return new NumericLiteral( a.number.doubleValue() - b.number.doubleValue() );
	}

	public static NumericLiteral mul( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
			return new NumericLiteral( a.number.intValue() * b.number.intValue() );
		else
			return new NumericLiteral( a.number.doubleValue() * b.number.doubleValue() );
	}

	// Note: does not check for divide-by-zero.
	public static NumericLiteral div( final NumericLiteral a,
										final NumericLiteral b )
	{
		if ( NumericLiteralType.INTEGER == a.type && NumericLiteralType.INTEGER == b.type )
			return new NumericLiteral( a.number.intValue() / b.number.intValue() );
		else
			return new NumericLiteral( a.number.doubleValue() / b.number.doubleValue() );
	}

/*
	// Note: does not check for divide-by-zero.
	public static NumericLiteral mod( final NumericLiteral a,
										final NumericLiteral b )
	{

	}

	public static NumericLiteral pow( final NumericLiteral a,
										final NumericLiteral b )
	{

	}
*/

	public int intValue()
	{
		return number.intValue();
	}

	public double doubleValue()
	{
		return number.doubleValue();
	}

	// RippleValue methods /////////////////////////////////////////////////////

	public RdfValue toRdf( ModelConnection mc )
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

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		p.print( number );
	}

	public int compareTo( final RippleValue other )
	{
		if ( other instanceof NumericLiteral )
		{
			double n = number.doubleValue(),
				nOther =  ((NumericLiteral) other ).number.doubleValue();
			return ( n > nOther ) ? 1 : ( n < nOther ) ? -1 : 0;
		}

		else if ( other instanceof RdfValue )
		{
return 0;

		}

		else
			return RippleList.class.getName().compareTo( other.getClass().getName() );
	}
}

// kate: tab-width 4
