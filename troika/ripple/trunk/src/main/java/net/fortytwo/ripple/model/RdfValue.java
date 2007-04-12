package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

/**
 *  Note: this class has a natural ordering that is inconsistent with equals.
 */
public class RdfValue implements RippleValue
{
	private Value value;
	
	public RdfValue( Value value )
	{
		this.value = value;
	}

	public Value getRdfValue()
	{
		return value;
	}

	public RdfValue toRdf( ModelConnection mc )
	{
		return this;
	}

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		p.print( value );
	}

	public boolean isActive()
	{
		return false;
	}

	public int compareTo( RippleValue other )
	{
//System.out.println( "[" + this + "].compareTo(" + other + ")" );
		if ( other instanceof RdfValue )
		{
			if ( this.value instanceof Literal )
			{
				if ( ( (RdfValue) other ).value instanceof Literal )
					return ( (Literal) this.value ).getLabel().compareTo(
						( (Literal) ( (RdfValue) other ).value ).getLabel() );
				else
					return value.getClass().getName().compareTo(
						( (RdfValue) other ).value.getClass().getName() );
			}

			else if ( this.value instanceof URI )
			{
				if ( ( (RdfValue) other ).value instanceof URI )
					return ( (URI) this.value ).toString().compareTo(
						( (URI) ( (RdfValue) other ).value ).toString() );
				else
					return value.getClass().getName().compareTo(
						( (RdfValue) other ).value.getClass().getName() );
			}

			// hack...
			else
			{
				int hcThis = value.hashCode();
				int hcOther = ( (RdfValue) other ).value.hashCode();
				if ( hcThis == hcOther )
					return 0;
				else if ( hcThis < hcOther )
					return -1;
				else
					return 1;
			}
		}

		else
			return RdfValue.class.getName().compareTo( other.getClass().getName() );
	}

	public boolean equals( Object other )
	{
		return ( other instanceof RdfValue )
			? value.equals( ( (RdfValue) other ).value )
			: false;
	}

	public int hashCode()
	{
		return value.hashCode();
	}

	public String toString()
	{
		// Use Sesame's toString() methods.
		return value.toString();
	}
}

// kate: tab-width 4
