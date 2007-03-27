package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class NameAst implements Ast
{
	private String localName;

	public NameAst( final String localName )
	{
		this.localName = localName;
	}

	public String toString()
	{
		return localName;
	}

	public void evaluate( Sink<RippleValue> sink,
						Interpreter itp,
						ModelConnection mc )
		throws RippleException
	{
		itp.resolveUnqualifiedName( localName, sink );
	}
}

// kate: tab-width 4
