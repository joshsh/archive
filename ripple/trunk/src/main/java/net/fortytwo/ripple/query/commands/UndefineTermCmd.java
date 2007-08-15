/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class UndefineTermCmd extends Command
{
	String term;

	public UndefineTermCmd( final String term )
	{
		this.term = term;
	}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		mc.removeStatementsAbout(
			mc.createUri( qe.getDefaultNamespace() + term ) );

		finished();
	}

	protected void abort() {}
}

// kate: tab-width 4
