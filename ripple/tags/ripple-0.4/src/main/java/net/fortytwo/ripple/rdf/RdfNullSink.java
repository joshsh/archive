/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfNullSink implements RdfSink
{
	private Sink<Statement> stSink = new NullSink<Statement>();
	private Sink<Namespace> nsSink = new NullSink<Namespace>();
	private Sink<String> cmtSink = new NullSink<String>();

	public Sink<Statement> statementSink()
	{
		return stSink;
	}

	public Sink<Namespace> namespaceSink()
	{
		return nsSink;
	}

	public Sink<String> commentSink()
	{
		return cmtSink;
	}
}

// kate: tab-width 4
