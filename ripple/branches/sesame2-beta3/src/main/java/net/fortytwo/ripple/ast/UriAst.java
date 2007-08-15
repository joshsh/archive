package net.fortytwo.ripple.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

public class UriAst implements Ast
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
			System.exit( 1 );
		}
	}

	public void evaluate( Sink<RippleValue> sink,
						QueryEngine qe,
						ModelConnection mc )
		throws RippleException
	{
		sink.put( mc.getModel().getBridge().get(
			new RdfValue( mc.createUri( value ) ) ) );
	}

	public String toString()
	{
		return "<" + StringUtils.escapeUriString( value ) + ">";
	}


}

// kate: tab-width 4
