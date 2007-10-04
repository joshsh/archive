/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Tee;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfTee implements RdfSink
{
	private Sink<Statement> statementTee;
	private Sink<Namespace> namespaceTee;
	private Sink<String> commentTee;

	public RdfTee( final RdfSink sinkA, final RdfSink sinkB )
	{
		statementTee = new Tee<Statement>( sinkA.statementSink(), sinkB.statementSink() );
		namespaceTee = new Tee<Namespace>( sinkA.namespaceSink(), sinkB.namespaceSink() );
		commentTee = new Tee<String>( sinkA.commentSink(), sinkB.commentSink() );
	}

	public Sink<Statement> statementSink()
	{
		return statementTee;
	}

	public Sink<Namespace> namespaceSink()
	{
		return namespaceTee;
	}

	public Sink<String> commentSink()
	{
		return commentTee;
	}
}

// kate: tab-width 4
