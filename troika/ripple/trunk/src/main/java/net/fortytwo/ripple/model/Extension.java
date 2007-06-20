package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.UrlFactory;

public interface Extension
{
	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
