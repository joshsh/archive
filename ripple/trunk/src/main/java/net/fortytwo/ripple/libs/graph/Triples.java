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
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

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
		URI uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		SesameInputAdapter sc = createAdapter( stack, sink );

		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, sc, uri.toString() );
	}

	static SesameInputAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink )
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
	
					RippleList triple = new RippleList( subj ).push( pred ).push( obj );
					resultSink.put(
						new RippleList( triple, stack ) );
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
