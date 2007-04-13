package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.cli.ParserQuitException;
import net.fortytwo.ripple.query.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;

public class QuitCmd implements Command
{
	public QuitCmd() {}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		throw new ParserQuitException();
	}
}

// kate: tab-width 4
