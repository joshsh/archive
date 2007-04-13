package net.fortytwo.ripple.cli.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;

public class UndefineTermCmd implements Command
{
	private String term;

	public UndefineTermCmd( final String term )
	{
		this.term = term;
	}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		mc.removeStatementsAbout(
			mc.createUri( qc.getDefaultNamespace() + term ) );
	}
}

// kate: tab-width 4
