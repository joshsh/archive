/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfCollector implements RdfSink, RdfSource
{
	private Collector<Statement> statements;
	private Collector<Namespace> namespaces;
	private Collector<String> comments;

	public RdfCollector()
	{
		statements = new Collector<Statement>();
		namespaces = new Collector<Namespace>();
		comments = new Collector<String>();
	}

	public Sink<Statement> statementSink()
	{
		return statements;
	}

	public Sink<Namespace> namespaceSink()
	{
		return namespaces;
	}

	public Sink<String> commentSink()
	{
		return comments;
	}

	public Source<Statement> statementSource()
	{
		return statements;
	}

	public Source<Namespace> namespaceSource()
	{
		return namespaces;
	}

	public Source<String> commentSource()
	{
		return comments;
	}

	public void writeTo( final RdfSink sink ) throws RippleException
	{
		statements.writeTo( sink.statementSink() );
		namespaces.writeTo( sink.namespaceSink() );
		comments.writeTo( sink.commentSink() );
	}

	public void clear()
	{
		statements.clear();
		namespaces.clear();
		comments.clear();
	}

	public int countStatements()
	{
		return statements.size();
	}

	public int countNamespaces()
	{
		return namespaces.size();
	}

	public int countComments()
	{
		return comments.size();
	}
}

// kate: tab-width 4
