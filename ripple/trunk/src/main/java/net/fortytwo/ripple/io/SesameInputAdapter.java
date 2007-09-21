/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Statement;
import org.openrdf.model.Namespace;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.rio.RDFHandler;

/**
 * An RDFHandler which passes its input into an RdfSink.
 */
public class SesameInputAdapter implements RDFHandler
{
	private Sink<Statement> stSink;
	private Sink<Namespace> nsSink;
	private Sink<String> cmtSink;

	public SesameInputAdapter( final RdfSink sink )
	{
		stSink = sink.statementSink();
		nsSink = sink.namespaceSink();
		cmtSink = sink.commentSink();
	}

	/**
	 *  Handles a comment.
	 */
	public void handleComment( final String comment )
	{
		try
		{
			cmtSink.put( comment );
		}

		catch ( RippleException e )
		{
			// Log the error, but continue.
			e.logError();
		}
	}

	/**
	 *  Handles a namespace declaration/definition.
	 */
	public void handleNamespace( final String prefix, final String uri )
	{
		try
		{
			nsSink.put( new NamespaceImpl( prefix, uri ) );
		}

		catch ( RippleException e )
		{
			// Log the error, but continue.
			e.logError();
		}
	}

	/**
	 *  Handles a statement.
	 */
	public void handleStatement( final Statement st )
	{
		try
		{
			stSink.put( st );
		}

		catch ( RippleException e )
		{
			// Log the error, but continue.
			e.logError();
		}
	}

	/**
	 *  Signals the start of the RDF data.
	 */
	public void startRDF()
	{
	}

	/**
	 *  Signals the end of the RDF data.
	 */
	public void endRDF()
	{
	}
}

// kate: tab-width 4
