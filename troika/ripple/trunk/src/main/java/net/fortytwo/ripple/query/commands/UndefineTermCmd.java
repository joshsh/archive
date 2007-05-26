package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class UndefineTermCmd implements Command
{
	private String term;

	public UndefineTermCmd( final String term )
	{
		this.term = term;
	}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		mc.removeStatementsAbout(
			mc.createUri( qe.getDefaultNamespace() + term ) );
	}
}

// kate: tab-width 4
