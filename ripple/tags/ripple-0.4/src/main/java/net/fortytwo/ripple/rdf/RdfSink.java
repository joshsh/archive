/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

import net.fortytwo.ripple.util.Sink;

public interface RdfSink
{
	Sink<Statement> statementSink();
	Sink<Namespace> namespaceSink();
	Sink<String> commentSink();
}

// kate: tab-width 4
