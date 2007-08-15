/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.RdfSink;

import org.openrdf.model.Statement;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;

public class RdfSourceAdapter implements RDFHandler
{
	RdfSink sink;

	public RdfSourceAdapter( RdfSink sink )
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
			// Do nothing.
		}
	}

	/**
	 *  Handles a namespace declaration/definition.
	 */
	public void handleNamespace( String prefix, String uri )
	{
		try
		{
			sink.put( new NamespaceImpl( prefix, uri ) );
		}

		catch ( RippleException e )
		{
			// Do nothing.
		}
	}

	/**
	 *  Handles a statement.
	 */
	public void handleStatement( Statement st )
	{
		try
		{
			sink.put( st );
		}

		catch ( RippleException e )
		{
			// Do nothing.
		}
	}

	/**
	 *  Signals the start of the RDF data.
	 */
	public void startRDF()
	{}

	/**
	 *  Signals the end of the RDF data.
	 */
	public void endRDF()
	{}
}

// kate: tab-width 4
