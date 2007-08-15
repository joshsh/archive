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
import net.fortytwo.ripple.io.RdfSourceAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfSink;
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

public class Namespaces extends PrimitiveFunction
{
	public Namespaces()
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
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		RdfSourceAdapter adapter = createAdapter( stack, sink, mc );

		URLConnection uc = HttpUtils.openConnection( uri.toString() );
		HttpUtils.prepareUrlConnectionForRdfRequest( uc );
		RdfUtils.read( uc, adapter, uri.toString() );
	}

	static RdfSourceAdapter createAdapter( final RippleList stack,
										final Sink<RippleList> resultSink,
										final ModelConnection mc )
	{
		RdfSink rdfSink = new RdfSink()
		{
			// Discard statements.
			public void put( final Statement st ) throws RippleException {}

			// Push namespaces to the stack as pairs.
			public void put( final Namespace ns ) throws RippleException
			{
				RippleList result = new RippleList(
					mc.createValue( ns.getPrefix() ) ).push( mc.createValue( ns.getName() ) );
				resultSink.put( new RippleList( result, stack ) );
			}

			// Discard comments.
			public void put( final String comment ) throws RippleException {}
		};

		RdfSourceAdapter adapter = new RdfSourceAdapter( rdfSink );

		return adapter;
	}
}

// kate: tab-width 4
