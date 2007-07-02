package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.ast.UriAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Collector;

import org.openrdf.model.URI;
import org.openrdf.model.impl.NamespaceImpl;

public class DefinePrefixCmd implements Command
{
	private String prefix;
	private UriAst uri;

	public DefinePrefixCmd( final String prefix, final UriAst uri )
	{
		this.prefix = prefix;
		this.uri = uri;
	}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		Collector<RippleValue> sink = new Collector<RippleValue>();
		uri.evaluate( sink, qe, mc );
		if ( sink.size() == 0 )
			throw new RippleException( "URI could not be constructed from " + uri );
		else if ( sink.size() > 1 )
			throw new RippleException( "multiple values constructed from " + uri );

		URI ns = mc.uriValue( sink.iterator().next() );
		mc.setNamespace( prefix, ns, true );
		mc = null;

		qe.getLexicon().add( new NamespaceImpl( prefix, ns.toString() ), true );
	}
}

// kate: tab-width 4
