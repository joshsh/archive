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
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

public class RippleQueryCmd extends Command
{
	private ListAst listAst;
	private Sink<RippleList> sink;
	private Source<RippleList> composedWith;
	private Evaluator evaluator;
	private Collector<RippleList> expressions;

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
		if ( null == expressions )
		{
			expressions = new Collector<RippleList>();

			final Sink<RippleList> exprSink = new Sink<RippleList>()
			{
				public void put( final RippleList l ) throws RippleException
				{
					// Note: the first element of the list will also be a list
					final RippleList stack = mc.invert( (RippleList) l.getFirst() );

					Sink<RippleList> composedWithSink = new Sink<RippleList>()
					{
						public void put( final RippleList base )
							throws RippleException
						{
							expressions.put( mc.concat( stack, base ) );
						}
					};

					composedWith.writeTo( composedWithSink );
				}
			};

			listAst.evaluate( exprSink, qe, mc );
		}

		evaluator = qe.getEvaluator();

		final Sink<StackContext> resultSink = new Sink<StackContext>() {

			public void put( final StackContext arg ) throws RippleException
			{
				sink.put( arg.getStack() );
			}
		};
		final Sink<RippleList> evaluatorSink = new Sink<RippleList>()
		{
			// Note: v will always be a list.
			public void put( final RippleList l ) throws RippleException
			{
				StackContext arg = new StackContext( l, mc );
				evaluator.applyTo( arg, resultSink );
			}
		};

//System.out.println( "evaluating: " + listAst );
		expressions.writeTo( evaluatorSink );		
	}

	protected void abort()
	{
//System.out.println( "aborting Ripple query command" );
		evaluator.stop();
	}
}

// kate: tab-width 4
