package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

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

	public boolean isOperator()
	{
		return false;
	}

	public boolean equals( RdfValue other )
	{
		return value.equals( other.value );
	}

	public String toString()
	{
		// Use Sesame's toString() methods.
		return value.toString();
	}
}

// kate: tab-width 4
