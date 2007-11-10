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
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.util.Sink;

public class ShowContextsCmd extends Command
{
	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		final RipplePrintStream ps = qe.getPrintStream();

		Sink<RippleValue> printSink = new Sink<RippleValue>()
		{
			private int i = 0;

			public void put( final RippleValue v ) throws RippleException
			{
				ps.print( "[" + i++ + "] " );
				ps.println( v );
			}
		};

		ps.println( "" );
		mc.putContexts( printSink );
		ps.println( "" );
	}

	protected void abort()
	{
	}
}

// kate: tab-width 4
