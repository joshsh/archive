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

public class CountStatementsCmd extends Command
{
	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		System.out.println( "\n" + qe.getModel().countStatements() + "\n" );
	}

	protected void abort() {}
}

// kate: tab-width 4
