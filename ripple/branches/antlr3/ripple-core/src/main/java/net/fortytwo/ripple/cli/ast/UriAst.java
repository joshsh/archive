/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.StringUtils;

public class UriAst implements Ast<RippleList>
{
	private String value;

	public UriAst( final String escapedValue )
	{
		try
		{
			this.value = StringUtils.unescapeUriString( escapedValue );
		}

		catch ( RippleException e )
		{
			e.logError();
			System.exit( 1 );
		}
	}

	public void evaluate( final Sink<RippleList, RippleException> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		sink.put( mc.list( mc.value( mc.createUri( value ) ) ) );
	}

	public String toString()
	{
		return "<" + StringUtils.escapeUriString( value ) + ">";
	}
}

// kate: tab-width 4
