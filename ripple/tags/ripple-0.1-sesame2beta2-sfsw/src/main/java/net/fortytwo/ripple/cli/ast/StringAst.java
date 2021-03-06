package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class StringAst implements Ast
{
	private String value, language;

	public StringAst( final String value )
	{
		this.value = value;
		this.language = null;
	}

	public StringAst( final String value, final String language )
	{
		this.value = value;
		this.language = language;
	}

	public void evaluate( Sink<RippleValue> sink,
						Interpreter itp,
						ModelConnection mc )
		throws RippleException
	{
		sink.put( ( null == language )
			? mc.createValue( value )
			: mc.createValue( value, language ) );
	}

	public String toString()
	{
		return "\"" + value + "\"" + ( ( null == language ) ? "" : ( "@" + language ) );
	}
}

// kate: tab-width 4
