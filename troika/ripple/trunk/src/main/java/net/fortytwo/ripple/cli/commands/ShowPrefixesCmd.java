package net.fortytwo.ripple.cli.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
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
