package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryContext;
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
