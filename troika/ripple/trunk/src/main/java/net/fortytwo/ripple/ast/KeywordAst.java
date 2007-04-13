package net.fortytwo.ripple.ast;

import net.fortytwo.ripple.query.QueryContext;
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
						QueryContext qc,
						ModelConnection mc )
		throws RippleException
	{
		qc.resolveKeyword( keyword, sink );
	}
}

// kate: tab-width 4
