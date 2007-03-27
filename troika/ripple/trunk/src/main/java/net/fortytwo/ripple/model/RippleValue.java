package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

public interface RippleValue
{
	public RdfValue toRdf( ModelConnection mc )
		throws RippleException;

	public boolean isOperator();

	public void printTo( RipplePrintStream p )
		throws RippleException;
}

// kate: tab-width 4
