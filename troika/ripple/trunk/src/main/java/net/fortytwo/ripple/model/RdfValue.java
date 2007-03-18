package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

public class RdfValue implements RippleValue
{
	private Value value;
	
	public RdfValue( Value value, ModelConnection mc )
	{
		this.value = value;
	}

	public Value getRdfValue()
	{
		return value;
	}

	public RippleValue toRdf( ModelConnection mc )
	{
		return this;
	}

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		p.print( value );
	}

	public boolean isFunctionEnvelope()
	{
		return false;
	}
}

// kate: tab-width 4
