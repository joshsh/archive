/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.graph;

import java.net.URLConnection;

import java.io.InputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.SesameCoupling;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.io.RdfSink;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

public class Triples extends PrimitiveFunction
{
	public Triples()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		URI uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		SesameCoupling sc = createAdapter( stack, sink );

		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, sc, uri.toString() );
	}

	static SesameCoupling createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink )
	{
		RdfSink rdfSink = new RdfSink()
		{
			// Push statements to the stack as triples.
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

			// Discard namespaces.
			public void put( final Namespace ns ) throws RippleException {}

			// Discard comments.
			public void put( final String comment ) throws RippleException {}
		};

		SesameCoupling sc = new SesameCoupling( rdfSink );

		return sc;
	}
}

// kate: tab-width 4
