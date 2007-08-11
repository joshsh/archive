package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.io.ParserQuitException;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class QuitCmd extends Command
{
	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
// FIXME...
		finished();

		throw new ParserQuitException();
	}

	protected void abort() {}
}

// kate: tab-width 4
