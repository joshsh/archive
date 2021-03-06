/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

import java.io.InputStream;

import java.net.URL;
import java.net.URLConnection;

import java.util.Iterator;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

/**
 * A primitive which consumes a literal value or a non-information resource and
 * produces a number of index results according to the Sindice service.  These
 * results represent Semantic Web documents which mention the given search
 * string (if a literal value) or URI (if a resource).
 */
public class Sindice extends PrimitiveFunction
{
	private static final int ARITY = 1;

	private static SAXBuilder saxBuilder = null;
	private static void initialize()
	{
		saxBuilder = new SAXBuilder( true );
		saxBuilder.setReuseParser( true );

		String schemaLocation = Sindice.class.getResource( "sindice.xsd" ).toString();
		saxBuilder.setFeature(
			"http://apache.org/xml/features/validation/schema", true );
		saxBuilder.setProperty( "http://apache.org/xml/properties/schema/"
			+ "external-noNamespaceSchemaLocation", schemaLocation );
	}

	public Sindice()
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
		if ( null == saxBuilder )
		{
			initialize();
		}

		Value v;

		v = stack.getFirst().toRdf( mc ).getRdfValue();
		stack = stack.getRest();

		String urlStr;
		if ( v instanceof Literal )
		{
			urlStr = "http://sindice.com/query/lookup?keyword="
//			urlStr = "http://sindice.com/keyword/"
//			urlStr = "http://sindice.com/beta/keyword/"
//			urlStr = "http://sindice.com/beta/lookup/keyword?keyword="
				+ StringUtils.percentEncode( v.toString() )
				+ "&type=keyword"
			;
		}

		else if ( v instanceof URI )
		{
			urlStr = "http://sindice.com/query/lookup?uri="
//			urlStr = "http://sindice.com/uri/"
//			urlStr = "http://sindice.com/lookup?uri="
//			urlStr = "http://sindice.com/beta/lookup_uri?uri="
				+ StringUtils.percentEncode( v.toString() )
//				+ "&lookup=Lookup"
				+ "&type=uri"
			;
		}

		else
		{
			throw new RippleException( "argument is neither a literal value nor a URI: " + v );
		}

		URLConnection urlConn;

/*
literals:
    http://sindice.com/beta/lookup_literal?keyword=timbl&lookup=Lookup
inverse functional properties:
    http://sindice.com/beta/lookup_ifp?property=http%3A%2F%2Fxmlns.com%2Ffoaf%2F0.1%2Fmbox&object=mailto%3Atimbl%40w3.org&lookup=Lookup
resources:
http://sindice.com/beta/lookup_uri?uri=http%3A%2F%2Fwww.w3.org%2FPeople%2FBerners-Lee%2Fcard%23i&lookup=Lookup
*/
		try
		{
			URL url = new URL( urlStr );
//			URL url = new URL( "http://www.sindice.com/lookup?uri="
//			URL url = new URL( "http://www.sindice.com/lookup/lookup_uri?uri="
//				+ StringUtils.percentEncode( uri.toString() ) );
			urlConn = url.openConnection();
		}

		catch ( java.net.MalformedURLException e )
		{
			throw new RippleException( e );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		String []mimeTypes = { "application/xml", "text/xml" };
		HttpUtils.prepareUrlConnectionForRequest( urlConn, mimeTypes );
		HttpUtils.connect( urlConn );

		Document doc;

		try
		{
			InputStream response = urlConn.getInputStream();

			synchronized ( saxBuilder )
			{
				doc = saxBuilder.build( response );
			}

			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		catch ( org.jdom.JDOMException e )
		{
			throw new RippleException( e );
		}

		Element root = doc.getRootElement();

		// Nonempty result set.
		if ( root.getName().equals( "sindice-sources" ) )
		{
			Iterator<Element> childIter = root.getChildren().iterator();
			while ( childIter.hasNext() )
			{
				Element sourceEl = childIter.next();
				Element urlEl = sourceEl.getChild( "url" );
				String s = urlEl.getText().trim();

				URI resultUri;

				// Tolerate bad document URIs, but don't make results out of them.
				try
				{
					resultUri = mc.createUri( s );
				}

				catch ( RippleException e )
				{
					e.logError();
					continue;
				}

				sink.put( new RippleList(
					new RdfValue( resultUri ), stack ) );
			}
		}

		// Nonempty result set (using the beta format)
		else if ( root.getName().equals( "records" ) )
		{
			Iterator<Element> childIter = root.getChildren().iterator();
			while ( childIter.hasNext() )
			{
				Element sourceEl = childIter.next();
				Element urlEl = sourceEl.getChild( "id" );
				String s = urlEl.getText().trim();

				URI resultUri;

				// Tolerate bad document URIs, but don't make results out of them.
				try
				{
					resultUri = mc.createUri( s );
				}

				catch ( RippleException e )
				{
					e.logError();
					continue;
				}

				sink.put( new RippleList(
					new RdfValue( resultUri ), stack ) );
			}
		}

		else if ( root.getName().equals( "nil-classes" ) )
		{
			// Do nothing.
			return;
		}

		else
		{
			throw new RippleException( "Unexpected Sindice response: " + root.getName() );
		}
	}
}

// kate: tab-width 4
