package net.fortytwo.ripple.io;

import java.io.InputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;

public class RdfSourceAdapter implements RDFHandler
{
	Sink<Statement> statementSink;
	Sink<Namespace> namespaceSink;
	Sink<String> commentSink;

	public RdfSourceAdapter( Sink<Statement> statementSink,
							Sink<Namespace> namespaceSink,
							Sink<String> commentSink )
	{
		this.statementSink = statementSink;
		this.namespaceSink = namespaceSink;
		this.commentSink = commentSink;
	}

	/**
	 *  Handles a comment.
	 */
	public void handleComment( final String comment )
	{
		try
		{
			commentSink.put( comment );
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
			namespaceSink.put( new NamespaceImpl( prefix, uri ) );
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
			statementSink.put( st );
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
