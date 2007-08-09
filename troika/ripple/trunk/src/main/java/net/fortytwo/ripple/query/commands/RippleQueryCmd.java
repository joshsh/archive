package net.fortytwo.ripple.query.commands;

import java.util.Collection;
import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.ast.ListAst;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.Evaluator;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

public class RippleQueryCmd implements Command
{
	ListAst listAst;
	Sink<RippleList> sink;
	Source<RippleList> composedWith;

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
		Sink<RippleValue> evaluatorSink = new Sink<RippleValue>()
		{
			// Note: v will always be a list.
			public void put( RippleValue v )
				throws RippleException
			{
				final RippleList l = (RippleList) v;
				final Evaluator evaluator = qe.getEvaluator();

				Sink<RippleList> composedWithSink = new Sink<RippleList>()
				{
					public void put( final RippleList base )
						throws RippleException
					{
						evaluator.applyTo(
							RippleList.concat( l, base ), sink, mc );
					}
				};

				composedWith.writeTo( composedWithSink );
			}
		};

		listAst.evaluate( evaluatorSink, qe, mc );
	}
}

// kate: tab-width 4
