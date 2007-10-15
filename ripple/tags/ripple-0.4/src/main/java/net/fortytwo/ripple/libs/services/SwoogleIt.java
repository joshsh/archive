/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.io.RdfImporter;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

import org.openrdf.model.URI;
import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.vocabulary.RDF;

import java.net.URL;

/**
 * A primitive which consumes a search string, a Swoogle search type (for
 * instance,
 * &lt;http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#search_swt&lt;)
 * and a Swoogle key (for instance, "demo") and produces a resource containing
 * Swoogle search results.
 */
public class SwoogleIt extends PrimitiveFunction
{
	private static final int ARITY = 3;

	private static final String SWOOGLE_NS
		= "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#";

	private URI swoogleQueryResponseUri = null;
	
	public SwoogleIt()
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
		if ( null == swoogleQueryResponseUri )
		{
			swoogleQueryResponseUri = mc.createUri( SWOOGLE_NS + "QueryResponse" );
		}

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
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st )
					throws RippleException
				{
					importer.statementSink().put( st );
	
					if ( st.getPredicate().equals( RDF.TYPE )
							&& st.getObject().equals( swoogleQueryResponseUri ) )
					{
						buffer.put( new RippleList(
							new RdfValue( st.getSubject() ), stackFinal ) );
					}
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns )
					throws RippleException
				{
					importer.namespaceSink().put( ns );
				}
			};

			private Sink<String> cmtSink = new Sink<String>()
			{
				public void put( final String comment )
					throws RippleException
				{
					importer.commentSink().put( comment );
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

		final SesameInputAdapter sc = new SesameInputAdapter( responseWatcher );

// TODO: enclose in a timeout wrapper
		RdfUtils.read( url, sc, url.toString() );

		buffer.flush();
	}
}

// kate: tab-width 4
