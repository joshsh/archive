package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class IntegerAst implements Ast
{
	private int value;

	public IntegerAst( final int value )
	{
		this.value = value;
	}

	public void evaluate( Sink<RippleValue> sink,
						QueryContext qc,
						ModelConnection mc )
		throws RippleException
	{
		sink.put( mc.createValue( value ) );
	}

	public String toString()
	{
		return "" + value;
	}
}

// kate: tab-width 4
