package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.util.Source;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public interface RdfSource
{
	Source<Statement> statementSource();
	Source<Namespace> namespaceSource();
	Source<String> commentSource();
}

// kate: tab-width 4
