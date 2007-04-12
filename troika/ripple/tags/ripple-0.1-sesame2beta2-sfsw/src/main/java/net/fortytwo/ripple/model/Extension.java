package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

public interface Extension
{
	public void load( ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
