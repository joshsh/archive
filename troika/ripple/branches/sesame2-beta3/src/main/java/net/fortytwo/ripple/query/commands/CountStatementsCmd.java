package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class CountStatementsCmd implements Command
{
	public CountStatementsCmd() {}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		System.out.println( "\n" + qe.getModel().countStatements() + "\n" );
	}
}

// kate: tab-width 4
