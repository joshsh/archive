/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/query/commands/UndefineTermCmd.java $
 * $Revision: 951 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.model.ModelConnection;

public class RedefineTermCmd extends Command
{
	private Command undefineCmd, defineCmd;

	public RedefineTermCmd( final String term, final ListAst ast )
	{
		undefineCmd = new UndefineTermCmd( term );
		defineCmd = new DefineTermCmd( term, ast );
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		undefineCmd.execute( qe, mc );
		defineCmd.execute( qe, mc );
	}

	protected void abort()
	{
	}
}

// kate: tab-width 4
