package net.fortytwo.ripple;

import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Function;

import java.net.URL;

public abstract class Extension
{
	public abstract void load( ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
