/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import java.net.URLConnection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

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
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		SesameInputAdapter sc = createAdapter( stack, sink, mc );

		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, sc, uri.toString() );
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
