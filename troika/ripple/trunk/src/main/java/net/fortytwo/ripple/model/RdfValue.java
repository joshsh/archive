package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;
import org.openrdf.model.Literal;

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

	public boolean equals( Object other )
	{
		if ( other instanceof RdfValue )
		{
			// Sesame's comparison for Literals appears to be either a little
			// too imprecise or a little too strict to be useful for searching
			// in Ripple, in that objects which should be equal are not, so we
			// special case it.
			if ( ( this.value instanceof Literal )
				&& ( ( (RdfValue) other ).value instanceof Literal ) )
			{
				return ( (Literal) this.value ).getLabel().equals(
					( (Literal) ( (RdfValue) other ).value ).getLabel() );
			}

			else
				return value.equals( ( (RdfValue) other ).value );
		}

		else
			return false;
	}

	public String toString()
	{
		// Use Sesame's toString() methods.
		return value.toString();
	}
}

// kate: tab-width 4
