/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.graph;

import java.net.URLConnection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.SesameAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.io.RdfSink;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

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

		SesameAdapter sc = createAdapter( stack, sink, mc );

		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, sc, uri.toString() );
	}

	static SesameAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink,
										final ModelConnection mc )
	{
		RdfSink rdfSink = new RdfSink()
		{
			// Discard statements.
			public void put( final Statement st ) throws RippleException
			{
			}

			// Discard namespaces.
			public void put( final Namespace ns ) throws RippleException
			{
			}

			// Push comments to the stack.
			public void put( final String comment ) throws RippleException
			{
				resultSink.put(
					new RippleList( mc.createValue( comment ), stack ) );
			}
		};

		SesameAdapter sc = new SesameAdapter( rdfSink );

		return sc;
	}
}

// kate: tab-width 4
