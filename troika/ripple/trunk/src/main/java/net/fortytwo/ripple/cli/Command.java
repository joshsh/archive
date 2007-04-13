package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public interface Command
{
	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
