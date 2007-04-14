package net.fortytwo.ripple.query.commands;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RippleValue;

public class ShowContextsCmd implements Command
{
	public ShowContextsCmd() {}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		RipplePrintStream ps = qe.getPrintStream();

		ps.println( "" );

		int i = 0;
		for ( Iterator<RippleValue> iter
			= qe.getModel().getContexts().iterator(); iter.hasNext(); )
		{
			ps.print( "[" + i++ + "] " );
			ps.println( iter.next() );
		}

		ps.println( "" );
	}
}

// kate: tab-width 4
