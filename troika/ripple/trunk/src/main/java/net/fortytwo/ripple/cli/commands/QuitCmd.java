package net.fortytwo.ripple.cli.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.ParserQuitException;
import net.fortytwo.ripple.cli.QueryContext;
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
