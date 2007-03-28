package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class KeywordAst implements Ast
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

	public void evaluate( Sink<RippleValue> sink,
						Interpreter itp,
						ModelConnection mc )
		throws RippleException
	{
		itp.resolveKeyword( keyword, sink );
	}
}

// kate: tab-width 4
