package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;

public class ShowPrefixesCmd implements Command
{
	public ShowPrefixesCmd() {}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		qc.getModel().showNamespaces();
	}
}

// kate: tab-width 4
