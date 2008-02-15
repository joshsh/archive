/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

public class KeywordAst implements Ast<RippleList>
{
	private String keyword;

	public KeywordAst( final String keyword )
	{
		this.keyword = keyword;
	}

	public String toString()
	{
		return keyword;
	}

	public void evaluate( final Sink<RippleList> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		Sink<RippleValue> uriSink = new Sink<RippleValue>()
		{
			public void put(final RippleValue v) throws RippleException
			{
				sink.put( mc.list( v ) );
			}
		};

		qe.uriForKeyword( keyword, uriSink, mc );
	}
}

// kate: tab-width 4
