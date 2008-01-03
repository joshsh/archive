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
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
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
 * A primitive which consumes an information resource and produces a list
 * (subject, predicate, object) for each RDF triple in the corresponding
 * Semantic Web document.
 */
public class Triples extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Triples()
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
		String uri = mc.toUri( stack.getFirst() ).toString();
		stack = stack.getRest();

		SesameInputAdapter sc = createAdapter( stack, sink, mc );

		HttpMethod method = HttpUtils.createGetMethod( uri );
		HttpUtils.setRdfAcceptHeader( method );
		RdfUtils.read( method, sc, uri, null );
	}

	private static SesameInputAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink,
										final ModelConnection mc )
	{
		RdfSink rdfSink = new RdfSink()
		{
			// Discard statements.
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st ) throws RippleException
				{
// Note: don't bother with the ModelBridge for now.
					RippleValue subj = new RdfValue( st.getSubject() );
					RippleValue pred = new RdfValue( st.getPredicate() );
					RippleValue obj = new RdfValue( st.getObject() );
	
					RippleList triple = mc.list( obj ).push( pred ).push( subj );
					resultSink.put(
						mc.list( triple, stack ) );
				}
			};

			// Discard namespaces.
			private Sink<Namespace> nsSink = new NullSink<Namespace>();

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
