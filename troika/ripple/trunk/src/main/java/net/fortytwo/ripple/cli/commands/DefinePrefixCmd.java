package net.fortytwo.ripple.cli.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.cli.ast.UriAst;
import net.fortytwo.ripple.model.ContainerSink;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.URI;

public class DefinePrefixCmd implements Command
{
	private String prefix;
	private UriAst uri;

	public DefinePrefixCmd( final String prefix, final UriAst uri )
	{
		this.prefix = prefix;
		this.uri = uri;
	}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		ContainerSink sink = new ContainerSink();
		uri.evaluate( sink, qc, mc );
		if ( sink.size() == 0 )
			throw new RippleException( "URI could not be constructed from " + uri );
		else if ( sink.size() > 1 )
			throw new RippleException( "multiple values constructed from " + uri );

		URI ns = mc.uriValue( sink.iterator().next() );
		mc.setNamespace( prefix, ns );
		mc = null;

		qc.getLexicon().update();
	}
}

// kate: tab-width 4
