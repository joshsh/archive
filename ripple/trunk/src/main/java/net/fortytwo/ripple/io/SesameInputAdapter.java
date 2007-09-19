/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Statement;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.rio.RDFHandler;

/**
 * An RDFHandler which passes its input into an RdfSink.
 */
public class SesameInputAdapter implements RDFHandler
{
	private RdfSink sink;

	public SesameInputAdapter( final RdfSink sink )
	{
		this.sink = sink;
	}

	/**
	 *  Handles a comment.
	 */
	public void handleComment( final String comment )
	{
		try
		{
			sink.put( comment );
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
			sink.put( new NamespaceImpl( prefix, uri ) );
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
			sink.put( st );
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
