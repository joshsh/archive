/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.util.Iterator;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class RdfCollector extends RdfSink
{
	private LinkedList<Statement> statements;
	private LinkedList<Namespace> namespaces;
	private LinkedList<String> comments;

	public RdfCollector()
	{
		statements = new LinkedList<Statement>();
		namespaces = new LinkedList<Namespace>();
		comments = new LinkedList<String>();
	}

	public void put( final Statement st ) throws RippleException
	{
		statements.add( st );
	}

	public void put( final Namespace ns ) throws RippleException
	{
		namespaces.add( ns );
	}

	public void put( final String comment ) throws RippleException
	{
		comments.add( comment );
	}

	public void writeTo( final RdfSink sink ) throws RippleException
	{
		Iterator<Statement> stmtIter = statements.iterator();
		while ( stmtIter.hasNext() )
		{
			sink.put( stmtIter.next() );
		}

		Iterator<Namespace> nsIter = namespaces.iterator();
		while ( nsIter.hasNext() )
		{
			sink.put( nsIter.next() );
		}

		Iterator<String> commentIter = comments.iterator();
		while ( commentIter.hasNext() )
		{
			sink.put( commentIter.next() );
		}
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
