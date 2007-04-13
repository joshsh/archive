package net.fortytwo.ripple.ast;

import net.fortytwo.ripple.query.QueryContext;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class BlankNodeAst implements Ast
{
	private String id;

	public BlankNodeAst( final String id )
	{
		this.id = id;
	}

	public String toString()
	{
		return "_:" + id;
	}

	public void evaluate( Sink<RippleValue> sink,
						QueryContext qc,
						ModelConnection mc )
		throws RippleException
	{
		RippleValue v = new RdfValue( mc.createBNode( id ) );

		if ( null == v )
			throw new RippleException( "blank node '" + this + "' does not exist" );
		else
			sink.put( v );
	}
}

// kate: tab-width 4
