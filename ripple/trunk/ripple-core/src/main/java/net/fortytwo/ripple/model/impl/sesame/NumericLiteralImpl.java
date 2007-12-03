package net.fortytwo.ripple.model.impl.sesame;

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.XMLSchema;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.NumericLiteral;
import net.fortytwo.ripple.model.RdfValue;

public class NumericLiteralImpl extends NumericLiteral {

	private RdfValue rdfEquivalent = null;

	public NumericLiteralImpl( final int i )
	{
		type = NumericLiteralType.INTEGER;
		number = new Integer( i );
	}
	
	public NumericLiteralImpl( final long l )
	{
		type = NumericLiteralType.LONG;
		number = new Long( l );
	}
	
	public NumericLiteralImpl( final double d )
	{
		type = NumericLiteralType.DOUBLE;
		number = new Double( d );
	}
	
	public NumericLiteralImpl( final RdfValue rdf )
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

		else if ( dataType.equals( XMLSchema.LONG ) )
		{
			try
			{
				type = NumericLiteralType.LONG;
				number = new Long( ( (Literal) v ).intValue() );
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
	
	public RdfValue toRdf( final ModelConnection mc )
		throws RippleException
	{
		if ( null == rdfEquivalent )
		{
			switch ( type )
			{
				case INTEGER:
					rdfEquivalent = new RdfValue( mc.createLiteral( number.intValue() ) );
					break;
				case LONG:
					rdfEquivalent = new RdfValue( mc.createLiteral( number.longValue() ) );
					break;
				case DOUBLE:
					rdfEquivalent = new RdfValue( mc.createLiteral( number.doubleValue() ) );
					break;
			}
		}
	
		return rdfEquivalent;
	}
	
	public NumericLiteral abs()
	{
		NumericLiteral a = this;
		
		if ( NumericLiteralType.INTEGER == a.getType() )
		{
			return new NumericLiteralImpl( Math.abs( a.intValue() ) );
		}

		else if ( NumericLiteralType.LONG == a.getType() )
		{
			return new NumericLiteralImpl( Math.abs( a.longValue() ) );
		}
		
		else
		{
			return new NumericLiteralImpl( Math.abs( a.doubleValue() ) );
		}
	}

	public NumericLiteral neg()
	{
		NumericLiteral a = this;

		if ( NumericLiteralType.INTEGER == a.getType() )
		{
			return new NumericLiteralImpl( -a.intValue() );
		}

		else if ( NumericLiteralType.LONG == a.getType() )
		{
			return new NumericLiteralImpl( -a.longValue() );
		}
		
		else
		{
			// Note: avoids negative zero.
			return new NumericLiteralImpl( 0.0 - a.doubleValue() );
		}
	}

	public NumericLiteral add( final NumericLiteral b )
	{
		NumericLiteral a = this;

		if ( NumericLiteralType.INTEGER == a.getType() && NumericLiteralType.INTEGER == b.getType() )
		{
			return new NumericLiteralImpl( a.intValue() + b.intValue() );
		}

		if ( NumericLiteralType.LONG == a.getType() && NumericLiteralType.LONG == b.getType() )
		{
			return new NumericLiteralImpl( a.longValue() + b.longValue() );
		}
		
		else
		{
			return new NumericLiteralImpl( a.doubleValue() + b.doubleValue() );
		}
	}

	public NumericLiteral sub( final NumericLiteral b )
	{
		NumericLiteral a = this;

		if ( NumericLiteralType.INTEGER == a.getType() && NumericLiteralType.INTEGER == b.getType() )
		{
			return new NumericLiteralImpl( a.intValue() - b.intValue() );
		}

		else if ( NumericLiteralType.LONG == a.getType() && NumericLiteralType.LONG == b.getType() )
		{
			return new NumericLiteralImpl( a.longValue() - b.longValue() );
		}
		
		else
		{
			return new NumericLiteralImpl( a.doubleValue() - b.doubleValue() );
		}
	}

	public NumericLiteral mul( final NumericLiteral b )
	{
		NumericLiteral a = this;

		if ( NumericLiteralType.INTEGER == a.getType() && NumericLiteralType.INTEGER == b.getType() )
		{
			return new NumericLiteralImpl( a.intValue() * b.intValue() );
		}

		if ( NumericLiteralType.LONG == a.getType() && NumericLiteralType.LONG == b.getType() )
		{
			return new NumericLiteralImpl( a.longValue() * b.longValue() );
		}
		
		else
		{
			return new NumericLiteralImpl( a.doubleValue() * b.doubleValue() );
		}
	}

	// Note: does not check for divide-by-zero.
	public NumericLiteral div( final NumericLiteral b )
	{
		NumericLiteral a = this;

		if ( NumericLiteralType.INTEGER == a.getType() && NumericLiteralType.INTEGER == b.getType() )
		{
			return new NumericLiteralImpl( a.intValue() / b.intValue() );
		}

		if ( NumericLiteralType.LONG == a.getType() && NumericLiteralType.LONG == b.getType() )
		{
			return new NumericLiteralImpl( a.longValue() / b.longValue() );
		}
		
		else
		{
			return new NumericLiteralImpl( a.doubleValue() / b.doubleValue() );
		}
	}

	// Note: does not check for divide-by-zero.
	public NumericLiteral mod( final NumericLiteral b )
	{
		NumericLiteral a = this;

		if ( NumericLiteralType.INTEGER == a.getType() && NumericLiteralType.INTEGER == b.getType() )
		{
			return new NumericLiteralImpl( a.intValue() % b.intValue() );
		}

		if ( NumericLiteralType.LONG == a.getType() && NumericLiteralType.LONG == b.getType() )
		{
			return new NumericLiteralImpl( a.longValue() % b.longValue() );
		}
		
		else
		{
			return new NumericLiteralImpl( a.doubleValue() % b.doubleValue() );
		}
	}

	public NumericLiteral pow( final NumericLiteral pow )
	{
		NumericLiteral a = this;

		double r = Math.pow( a.doubleValue(), pow.doubleValue() );

		if ( NumericLiteralType.INTEGER == a.getType() && NumericLiteralType.INTEGER == pow.getType() )
		{
			return new NumericLiteralImpl( (int) r );
		}

		if ( NumericLiteralType.LONG == a.getType() && NumericLiteralType.LONG == pow.getType() )
		{
			return new NumericLiteralImpl( (long) r );
		}
		
		else
		{
			return new NumericLiteralImpl( r );
		}
	}
}
