package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public interface Extension
{
	public void load( UrlFactory uf, ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
