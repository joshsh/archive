package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Source;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public abstract class RdfSource
{
	public abstract Source<Statement> statementSource();
	public abstract Source<Namespace> namespaceSource();
	public abstract Source<String> commentSource();

	public void writeTo( final RdfSink sink ) throws RippleException
	{
		statementSource().writeTo( sink.statementSink() );
		namespaceSource().writeTo( sink.namespaceSink() );
		commentSource().writeTo( sink.commentSink() );
	}
}

// kate: tab-width 4
