/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.SynchronizedSink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class SynchronizedRdfSink implements RdfSink
{
	private Sink<Statement> stSink;
	private Sink<Namespace> nsSink;
	private Sink<String> comSink;

	public SynchronizedRdfSink( final RdfSink sink )
	{
		stSink = new SynchronizedSink<Statement>( sink.statementSink() );
		nsSink = new SynchronizedSink<Namespace>( sink.namespaceSink() );
		comSink = new SynchronizedSink<String>( sink.commentSink() );
	}

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
		return comSink;
	}
}

// kate: tab-width 4
