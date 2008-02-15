/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import java.util.Iterator;

import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class TypedLiteralAst implements Ast<RippleList>
{
	private String value;
	private Ast type;

	public TypedLiteralAst( final String value, final Ast type )
	{
		this.value = value;
		this.type = type;
	}

	public void evaluate( final Sink<RippleList> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		Sink<RippleValue> typeSink = new Sink<RippleValue>()
		{
			public void put( final RippleValue type ) throws RippleException
			{
				sink.put( mc.list( mc.createTypedLiteral( value, type ) ) );
			}
		};
		
		type.evaluate( typeSink, qe, mc );
	}

	public String toString()
	{
		return "\"" + value + "\"^^" + type;
	}
}

// kate: tab-width 4
