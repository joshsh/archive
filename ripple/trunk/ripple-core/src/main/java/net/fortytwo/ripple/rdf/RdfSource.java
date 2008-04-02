/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Source;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public abstract class RdfSource
{
	public abstract Source<Statement, RippleException> statementSource();
	public abstract Source<Namespace, RippleException> namespaceSource();
	public abstract Source<String, RippleException> commentSource();

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
