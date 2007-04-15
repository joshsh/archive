package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.io.ParserQuitException;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class QuitCmd implements Command
{
	public QuitCmd() {}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		throw new ParserQuitException();
	}
}

// kate: tab-width 4
