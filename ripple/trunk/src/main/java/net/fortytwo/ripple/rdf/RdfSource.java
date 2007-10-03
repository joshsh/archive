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
		commentSource().writeTo( sink.commentSink() );

		// Note: it's often important that namespaces are written before
		// statements.
		namespaceSource().writeTo( sink.namespaceSink() );

		statementSource().writeTo( sink.statementSink() );
	}
}

// kate: tab-width 4
