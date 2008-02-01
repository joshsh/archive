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
import net.fortytwo.ripple.model.Context;
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
public class Namespaces extends PrimitiveFunction
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

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		String uri;

		uri = mc.toUri( stack.getFirst() ).toString();
		stack = stack.getRest();

		SesameInputAdapter sc = createAdapter( stack, sink, mc );

		HttpMethod method = HttpUtils.createGetMethod( uri );
		HttpUtils.setRdfAcceptHeader( method );
		RdfUtils.read( method, sc, uri, null );
		/*
		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, sc, uri.toString() );*/
	}

	private static SesameInputAdapter createAdapter( final RippleList stack,
												final Sink<RippleList> resultSink,
												final ModelConnection mc )
	{
		RdfSink rdfSink = new RdfSink()
		{
			// Discard statements.
			private Sink<Statement> stSink = new NullSink<Statement>();

			// Push namespaces to the stack as pairs.
			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns )
					throws RippleException
				{
					RippleList result = mc.list(
						mc.value( ns.getPrefix() ) ).push( mc.value( ns.getName() ) );
					resultSink.put( stack.push( result ) );
				}
			};

			// Discard comments.
			private Sink<String> cmtSink = new NullSink<String>();

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
