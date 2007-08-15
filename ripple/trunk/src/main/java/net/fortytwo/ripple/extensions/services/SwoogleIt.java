/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.services;


import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfImporter;
import net.fortytwo.ripple.model.RdfSink;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.model.vocabulary.RDF;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.net.URL;

public class SwoogleIt extends PrimitiveFunction
{
	static String swoogleNs = "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#";
	URI swoogleQueryResponseUri = null;
	
	public SwoogleIt()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 3;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		if ( null == swoogleQueryResponseUri )
			swoogleQueryResponseUri = mc.createUri( swoogleNs + "QueryResponse" );

		String key, searchString;
		URI queryType;

		key = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		queryType = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();
		searchString = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		URL url;

		try
		{
			String urlStr = "http://logos.cs.umbc.edu:8080/swoogle31/q"
				+ "?key=" + StringUtils.urlEncode( key )
				+ "&queryType=" + StringUtils.urlEncode( queryType.getLocalName() )
				+ "&searchString=" + StringUtils.urlEncode( searchString );

			url = new URL( urlStr );
		}

		catch ( java.net.MalformedURLException e )
		{
			throw new RippleException( e );
		}

		final RippleList stackFinal = stack;
URI context = mc.createUri( url.toString() );
		final RdfImporter importer = new RdfImporter( mc, context );

		// Output is buffered so that the entire document is imported into the
		// model before results are processed.
		final Buffer<RippleList> buffer = new Buffer<RippleList>( sink );

		RdfSink responseWatcher = new RdfSink()
		{
			public void put( final Statement st )
				throws RippleException
			{
				importer.put( st );

				if ( st.getPredicate().equals( RDF.TYPE )
					&& st.getObject().equals( swoogleQueryResponseUri ) )
					buffer.put( new RippleList(
						new RdfValue( st.getSubject() ), stackFinal ) );
			}

			public void put( final Namespace ns )
				throws RippleException
			{
				importer.put( ns );
			}

			public void put( final String comment )
				throws RippleException
			{
				importer.put( comment );
			}
		};

		final RdfSourceAdapter adapter = new RdfSourceAdapter( responseWatcher );

// TODO: enclose in a timeout wrapper
		RdfUtils.read( url, adapter, url.toString() );

		buffer.flush();
	}
}

// kate: tab-width 4
