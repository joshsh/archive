/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;

import org.apache.commons.httpclient.HttpMethod;
import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

/**
 * A primitive which consumes an information resource and produces a two-element
 * list (prefix name) for each namespace defined in the corresponding Semantic
 * Web document.
 */
public class Namespaces extends PrimitiveStackMapping
{
	private static final int ARITY = 1;

	public Namespaces()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		String uri;

		uri = mc.toUri( stack.getFirst() ).toString();
		stack = stack.getRest();

		SesameInputAdapter sc = createAdapter( arg, sink );

		HttpMethod method = HttpUtils.createGetMethod( uri );
		HttpUtils.setRdfAcceptHeader( method );
		RdfUtils.read( method, sc, uri, null );
		/*
		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, sc, uri.toString() );*/
	}

	static SesameInputAdapter createAdapter( final StackContext arg,
										final Sink<StackContext> resultSink )
	{
		final ModelConnection mc = arg.getModelConnection();
		final RippleList stack = arg.getStack();

		RdfSink rdfSink = new RdfSink()
		{
			// Discard statements.
			private Sink<Statement> stSink = new NullSink<Statement>();

			// Discard namespaces.
			private Sink<Namespace> nsSink = new NullSink<Namespace>();

			// Push comments.
			private Sink<String> cmtSink = new Sink<String>()
			{
				public void put( final String comment )
					throws RippleException
				{
					resultSink.put( arg.with(
						stack.push( mc.value( comment ) ) ) );
				}
			};

			public Sink<Statement> statementSink()
			{
				return stSink;
			}

			public Sink<Namespace> namespaceSink()
			{
				return nsSink;
			}

			public Sink<String> commentSink()
			{
				return cmtSink;
			}
		};

		SesameInputAdapter sc = new SesameInputAdapter( rdfSink );

		return sc;
	}
}

// kate: tab-width 4
