package net.fortytwo.ripple.query.commands;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;

import org.openrdf.model.Statement;

// E.g.
//      CONSTRUCT * FROM {x} p {y}
public class SerqlQueryCmd implements Command
{
	private String query;

	public SerqlQueryCmd( final String query )
	{
		this.query = query;
	}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		RipplePrintStream ps = qe.getPrintStream();
		Iterator<Statement> stmtIter = mc.serqlQuery( query ).iterator();

		ps.println( "" );
		ps.print( stmtIter );
		ps.println( "" );
	}
}

// kate: tab-width 4
