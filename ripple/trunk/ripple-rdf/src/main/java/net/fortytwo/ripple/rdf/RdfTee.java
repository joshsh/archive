/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-utils/src/main/java/net/fortytwo/ripple/rdf/RdfTee.java $
 * $Revision: 1042 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.flow.Tee;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfTee implements RdfSink
{
	private Sink<Statement, RippleException> statementTee;
	private Sink<Namespace, RippleException> namespaceTee;
	private Sink<String, RippleException> commentTee;

	public RdfTee( final RdfSink sinkA, final RdfSink sinkB )
	{
		statementTee = new Tee<Statement, RippleException>( sinkA.statementSink(), sinkB.statementSink() );
		namespaceTee = new Tee<Namespace, RippleException>( sinkA.namespaceSink(), sinkB.namespaceSink() );
		commentTee = new Tee<String, RippleException>( sinkA.commentSink(), sinkB.commentSink() );
	}

	public Sink<Statement, RippleException> statementSink()
	{
		return statementTee;
	}

	public Sink<Namespace, RippleException> namespaceSink()
	{
		return namespaceTee;
	}

	public Sink<String, RippleException> commentSink()
	{
		return commentTee;
	}
}

// kate: tab-width 4
