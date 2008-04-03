/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.NullSink;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.util.HttpUtils;

import org.apache.commons.httpclient.HttpMethod;
import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

/**
 * A primitive which consumes an information resource and produces a list
 * (subject, predicate, object) for each RDF triple in the corresponding
 * Semantic Web document.
 */
public class Triples extends PrimitiveStackMapping
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink
	)
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		String uri = mc.toUri( stack.getFirst() ).toString();

		SesameInputAdapter sc = createAdapter( arg, sink );

		HttpMethod method = HttpUtils.createGetMethod( uri );
		HttpUtils.setRdfAcceptHeader( method );
		RdfUtils.read( method, sc, uri, null );
	}

	static SesameInputAdapter createAdapter( final StackContext arg,
										final Sink<StackContext, RippleException> resultSink )
	{
		final ModelConnection mc = arg.getModelConnection();
		final RippleList rest = arg.getStack().getRest();
        final ModelBridge bridge = mc.getModel().getBridge();

		RdfSink rdfSink = new RdfSink()
		{
			// Push statements.
			private Sink<Statement, RippleException> stSink = new Sink<Statement, RippleException>()
            {
                public void put( final Statement st ) throws RippleException
                {
                    resultSink.put( arg.with(
                            rest.push( bridge.get( new RdfValue( st.getSubject() ) ) )
                                    .push( bridge.get( new RdfValue( st.getPredicate() ) ) )
                                    .push( bridge.get( new RdfValue( st.getObject() ) ) ) ) );
                }
            };

			// Discard namespaces.
			private Sink<Namespace, RippleException> nsSink = new NullSink<Namespace, RippleException>();

			// Discard comments.
			private Sink<String, RippleException> cmtSink = new NullSink<String, RippleException>();

			public Sink<Statement, RippleException> statementSink()
			{
				return stSink;
			}

			public Sink<Namespace, RippleException> namespaceSink()
			{
				return nsSink;
			}

			public Sink<String, RippleException> commentSink()
			{
				return cmtSink;
			}
		};

		SesameInputAdapter sc = new SesameInputAdapter( rdfSink );

		return sc;
	}
}

// kate: tab-width 4
