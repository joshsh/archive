/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

public class RippleQueryCmd extends Command
{
	private ListAst listAst;
	private Sink<RippleList> sink;
	private Source<RippleList> composedWith;
	private Evaluator evaluator;

	public RippleQueryCmd( final ListAst listAst,
							final Sink<RippleList> sink,
							final Source<RippleList> composedWith )
	{
		this.listAst = listAst;
		this.sink = sink;
		this.composedWith = composedWith;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		evaluator = qe.getEvaluator();

		final Sink<RippleValue> evaluatorSink = new Sink<RippleValue>()
		{
			// Note: v will always be a list.
			public void put( final RippleValue v ) throws RippleException
			{
				final RippleList stack = mc.invert( (RippleList) v );

				Sink<RippleList> composedWithSink = new Sink<RippleList>()
				{
					public void put( final RippleList base )
						throws RippleException
					{
						evaluator.applyTo(
							mc.concat( stack, base ), sink, mc );
					}
				};

				composedWith.writeTo( composedWithSink );
			}
		};

		listAst.evaluate( evaluatorSink, qe, mc );
	}

	protected void abort()
	{
//System.out.println( "aborting Ripple query command" );
		evaluator.stop();
	}
}

// kate: tab-width 4
