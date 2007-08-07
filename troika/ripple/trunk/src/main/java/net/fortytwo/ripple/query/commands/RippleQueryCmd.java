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

public class RippleQueryCmd implements Command
{
	ListAst listAst;
	Sink<RippleList> sink;
	Collection<RippleList> composedWith;

	public RippleQueryCmd( final ListAst listAst,
							final Sink<RippleList> sink,
							final Collection<RippleList> composedWith )
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
				RippleList l = (RippleList) v;
				Iterator<RippleList> iter = composedWith.iterator();
				while ( iter.hasNext() )
					qe.getEvaluator().applyTo(
						RippleList.concat( l, iter.next() ), sink, mc );
			}
		};

		listAst.evaluate( evaluatorSink, qe, mc );
	}
}

// kate: tab-width 4
