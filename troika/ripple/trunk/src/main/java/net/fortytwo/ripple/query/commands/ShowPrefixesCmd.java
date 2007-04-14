package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class ShowPrefixesCmd implements Command
{
	public ShowPrefixesCmd() {}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		qe.getModel().showNamespaces();
	}
}

// kate: tab-width 4
