/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Buffer;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.io.RDFImporter;
import net.fortytwo.ripple.rdf.RDFSink;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.StringUtils;
import net.fortytwo.ripple.util.RDFHTTPUtils;

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
public class SwoogleIt extends PrimitiveStackMapping
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink )
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		if ( null == swoogleQueryResponseUri )
		{
			swoogleQueryResponseUri = mc.createUri( SWOOGLE_NS + "QueryResponse" );
		}

		String key, searchString;
		URI queryType;

		key = mc.toString( stack.getFirst() );
		stack = stack.getRest();
		queryType = mc.toUri( stack.getFirst() );
		stack = stack.getRest();
		searchString = mc.toString( stack.getFirst() );
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
URI ctx = mc.createUri( url.toString() );
		final RDFImporter importer = new RDFImporter( mc, ctx );

		// Output is buffered so that the entire document is imported into the
		// model before results are processed.
		final Buffer<StackContext, RippleException> buffer = new Buffer<StackContext, RippleException>( sink );

		RDFSink responseWatcher = new RDFSink()
		{
			private Sink<Statement, RippleException> stSink = new Sink<Statement, RippleException>()
			{
				public void put( final Statement st )
					throws RippleException
				{
					importer.statementSink().put( st );
	
					if ( st.getPredicate().equals( RDF.TYPE )
							&& st.getObject().equals( swoogleQueryResponseUri ) )
					{
						buffer.put( arg.with(
								stackFinal.push( new RdfValue( st.getSubject() ) ) ) );
					}
				}
			};

			private Sink<Namespace, RippleException> nsSink = new Sink<Namespace, RippleException>()
			{
				public void put( final Namespace ns )
					throws RippleException
				{
					importer.namespaceSink().put( ns );
				}
			};

			private Sink<String, RippleException> cmtSink = new Sink<String, RippleException>()
			{
				public void put( final String comment )
					throws RippleException
				{
					importer.commentSink().put( comment );
				}
			};

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

		final SesameInputAdapter sc = new SesameInputAdapter( responseWatcher );

// TODO: enclose in a timeout wrapper
		RDFHTTPUtils.read( url, sc, url.toString() );

		buffer.flush();
	}
}

// kate: tab-width 4
