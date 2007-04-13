package net.fortytwo.ripple.cli.commands;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RippleValue;

public class ShowContextsCmd implements Command
{
	public ShowContextsCmd() {}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		RipplePrintStream ps = qc.getPrintStream();

		ps.println( "" );

		int i = 0;
		for ( Iterator<RippleValue> iter
			= qc.getModel().getContexts().iterator(); iter.hasNext(); )
		{
			ps.print( "[" + i++ + "] " );
			ps.println( iter.next() );
		}

		ps.println( "" );
	}
}

// kate: tab-width 4
