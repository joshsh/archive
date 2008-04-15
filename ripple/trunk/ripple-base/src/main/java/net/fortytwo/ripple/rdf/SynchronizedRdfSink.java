/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.flow.SynchronizedSink;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class SynchronizedRdfSink implements RdfSink
{
	private Sink<Statement, RippleException> stSink;
	private Sink<Namespace, RippleException> nsSink;
	private Sink<String, RippleException> comSink;

	public SynchronizedRdfSink( final RdfSink sink, final Object synch )
	{
		stSink = new SynchronizedSink<Statement, RippleException>( sink.statementSink(), synch );
		nsSink = new SynchronizedSink<Namespace, RippleException>( sink.namespaceSink(), synch );
		comSink = new SynchronizedSink<String, RippleException>( sink.commentSink(), synch );
	}

	public Sink<Statement, RippleException> statementSink()
	{
		return stSink;
	}

	public Sink<Namespace, RippleException> namespaceSink()
	{
		return nsSink;
	}

	public Sink<String, RippleException> commentSink()
	{
		return comSink;
	}
}

// kate: tab-width 4
