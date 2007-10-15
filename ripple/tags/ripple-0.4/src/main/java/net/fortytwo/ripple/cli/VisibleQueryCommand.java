/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.control.TaskSet;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.CollectorHistory;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Switch;
import net.fortytwo.ripple.util.SynchronizedSink;
import net.fortytwo.ripple.util.Tee;

import org.openrdf.model.Statement;

public class VisibleQueryCommand extends Command
{
	private ListAst ast;
	private CollectorHistory<RippleList> resultHistory;
	private boolean continued;
	private TaskSet taskSet;

	private Switch<RippleList> results;

	public VisibleQueryCommand( final ListAst query,
							final CollectorHistory<RippleList> history,
							final boolean continued )
	{
		ast = query;
		resultHistory = history;
		this.continued = continued;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		boolean doBuffer = Ripple.bufferQueryResults();

		qe.getPrintStream().println( "" );

		// Results are first dereferenced, then placed into a buffer which
		// will be flushed into the view after the lexicon is updated.
		TurtleView view = new TurtleView(
			qe.getPrintStream(), mc );

		Sink<RippleList> med = new SynchronizedSink(
			( doBuffer
				? new Buffer<RippleList>( view )
				: view ) );

		results = new Switch(
			new Tee<RippleList>( med, resultHistory ),
			new NullSink<RippleList>() );

		Sink<RippleList> derefSink = new Sink<RippleList>()
		{
			public void put( final RippleList list) throws RippleException
			{
				dereference( list.getFirst(), mc );
				results.put( list );
			}
		};

Collector<RippleList> nilSource = new Collector<RippleList>();
nilSource.put( RippleList.NIL );
		Command cmd = new RippleQueryCmd( ast, derefSink,
			( continued
				? resultHistory.getSource( 1 )
				: nilSource ) );

		// Execute the inner command and wait until it is finished.
		cmd.setQueryEngine( qe );
		taskSet = new TaskSet();
		taskSet.add( cmd );
		taskSet.waitUntilEmpty();

		// Flush results to the view.
		if ( doBuffer )
		{
			( (Buffer<RippleList>) med ).flush();
		}

		if ( view.size() > 0 )
		{
			qe.getPrintStream().println( "" );
		}

		resultHistory.advance();
	}

	protected void abort()
	{
		// Late arrivals should not show up in the view.
		results.flip();

		taskSet.stopWaiting();
	}

	private static void dereference( final RippleValue v, final ModelConnection mc )
		throws RippleException
	{
		RdfValue r = v.toRdf( mc );

		try
		{
mc.getStatements( r, null, null, new NullSink<Statement>() );
		}

		catch ( RippleException e )
		{
			// (soft fail... don't even log the error)
			return;
		}
	}
}

// kate: tab-width 4
