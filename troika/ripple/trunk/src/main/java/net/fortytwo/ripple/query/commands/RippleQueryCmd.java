package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.ast.ListAst;
import net.fortytwo.ripple.model.Evaluator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Sink;

public class RippleQueryCmd implements Command
{
	private ListAst listAst;
	private Sink<RippleList> sink;
	private Evaluator evaluator;

	public RippleQueryCmd( final ListAst listAst,
							final Evaluator evaluator,
							final Sink<RippleList> sink )
	{
		this.listAst = listAst;
		this.evaluator = evaluator;
		this.sink = sink;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		Sink<RippleValue> evaluatorSink = new Sink<RippleValue>()
		{
			public void put( RippleValue v )
				throws RippleException
			{
				evaluator.applyTo( (RippleList) v, sink, mc );
			}
		};

		listAst.evaluate( evaluatorSink, qe, mc );
	}
}

// kate: tab-width 4
