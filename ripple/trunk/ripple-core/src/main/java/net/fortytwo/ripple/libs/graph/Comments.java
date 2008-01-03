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
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
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
 * A primitive which consumes an information resource and produces each of
 * the comments matched in the corresponding RDF document. Note: with the
 * current Sesame bindings, nothing is actually matched.
 */
public class Comments extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Comments()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		String uri;

		uri = mc.toUri( stack.getFirst() ).toString();
		stack = stack.getRest();

		SesameInputAdapter sc = createAdapter( stack, sink, mc );

		HttpMethod method = HttpUtils.createGetMethod( uri );
		HttpUtils.setRdfAcceptHeader( method );
		RdfUtils.read( method, sc, uri, null );
	}

	static SesameInputAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink,
										final ModelConnection mc )
	{
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
					resultSink.put(
						mc.list( mc.value( comment ), stack ) );
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
