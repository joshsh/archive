package net.fortytwo.ripple.cli.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;

public class CountStatementsCmd implements Command
{
	public CountStatementsCmd() {}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		System.out.println( "\n" + qc.getModel().countStatements() + "\n" );
	}
}

// kate: tab-width 4
