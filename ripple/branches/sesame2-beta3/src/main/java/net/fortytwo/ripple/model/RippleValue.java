package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;

public interface RippleValue extends Comparable<RippleValue>
{
	public RdfValue toRdf( ModelConnection mc )
		throws RippleException;

	public boolean isActive();

	public void printTo( RipplePrintStream p )
		throws RippleException;
}

// kate: tab-width 4
