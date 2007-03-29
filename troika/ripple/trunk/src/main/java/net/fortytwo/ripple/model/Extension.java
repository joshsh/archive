package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

public abstract class Extension
{
	public abstract void load( ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
