package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Sink;

public class UriAst implements Ast
{
	private String value;

	public UriAst( final String value )
	{
		this.value = value;
	}

	public void evaluate( Sink<RippleValue> sink,
						Interpreter itp,
						ModelConnection mc )
		throws RippleException
	{
		sink.put( mc.getModel().getBridge().get(
			new RdfValue( mc.createUri( value ) ) ) );
	}

	public String toString()
	{
		return "<" + value + ">";
	}
}

// kate: tab-width 4
