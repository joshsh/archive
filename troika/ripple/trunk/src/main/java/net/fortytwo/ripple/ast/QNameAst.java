package net.fortytwo.ripple.ast;

import net.fortytwo.ripple.query.QueryContext;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Value;

public class QNameAst implements Ast
{
	private String nsPrefix, localName;

	public QNameAst( final String nsPrefix, final String localName )
	{
		this.nsPrefix = nsPrefix;
		this.localName = localName;
	}

	public String toString()
	{
		return ( ( null == nsPrefix ) ? "" : nsPrefix )
			+ ":"
			+ ( ( null == localName ) ? "" : localName );
	}

	public void evaluate( Sink<RippleValue> sink,
						QueryContext qc,
						ModelConnection mc )
		throws RippleException
	{
		qc.resolveQName( nsPrefix, localName, sink );
	}
}

// kate: tab-width 4
