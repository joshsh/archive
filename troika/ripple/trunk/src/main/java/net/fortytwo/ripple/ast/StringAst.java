package net.fortytwo.ripple.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

public class StringAst implements Ast
{
	private String value, language;

	public StringAst( final String escapedValue )
	{
		try
		{
			value = StringUtils.unescapeString( escapedValue );
		}

		catch ( RippleException e )
		{
			System.exit( 1 );
		}

		language = null;
	}

	public StringAst( final String escapedValue, final String language )
	{
		try
		{
			value = StringUtils.unescapeString( escapedValue );
		}

		catch ( RippleException e )
		{
			System.exit( 1 );
		}

		this.language = language;
	}

	public void evaluate( Sink<RippleValue> sink,
						QueryEngine qe,
						ModelConnection mc )
		throws RippleException
	{
		sink.put( ( null == language )
			? mc.createValue( value )
			: mc.createValue( value, language ) );
	}

	public String toString()
	{
		return "\"" + StringUtils.escapeString( value ) + "\""
			+ ( ( null == language ) ? "" : ( "@" + language ) );
	}
}

// kate: tab-width 4
