package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public interface Command
{
	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
