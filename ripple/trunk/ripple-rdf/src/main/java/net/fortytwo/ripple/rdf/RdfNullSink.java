/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-utils/src/main/java/net/fortytwo/ripple/rdf/RdfNullSink.java $
 * $Revision: 1042 $
 * $Author: josh $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.flow.NullSink;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfNullSink implements RdfSink
{
	private Sink<Statement, RippleException> stSink = new NullSink<Statement, RippleException>();
	private Sink<Namespace, RippleException> nsSink = new NullSink<Namespace, RippleException>();
	private Sink<String, RippleException> cmtSink = new NullSink<String, RippleException>();

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
		return cmtSink;
	}
}

// kate: tab-width 4
