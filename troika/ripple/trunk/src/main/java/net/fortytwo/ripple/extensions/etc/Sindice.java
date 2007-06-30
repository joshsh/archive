package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

import java.io.InputStream;

import java.net.URL;
import java.net.URLConnection;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

import org.openrdf.model.Literal;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Sindice extends PrimitiveFunction
{
	private static SAXBuilder s_saxBuilder = null;

	public Sindice( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		if ( null == s_saxBuilder )
		{
			s_saxBuilder = new SAXBuilder( true );
			s_saxBuilder.setReuseParser( true );

			String schemaLocation = PingTheSemanticWeb.class.getResource( "sindice.xsd" ).toString();
			s_saxBuilder.setFeature(
				"http://apache.org/xml/features/validation/schema", true );
			s_saxBuilder.setProperty( "http://apache.org/xml/properties/schema/"
				+ "external-noNamespaceSchemaLocation", schemaLocation );
		}

		Value v;

		v = stack.getFirst().toRdf( mc ).getRdfValue();
		stack = stack.getRest();

		String urlStr;
		if ( v instanceof Literal )
			urlStr = "http://sindice.com/beta/lookup_literal?keyword="
				+ StringUtils.urlEncode( v.toString() )
				+ "&lookup=Lookup";
		else if ( v instanceof URI )
			urlStr = "http://sindice.com/lookup?uri="
//			urlStr = "http://sindice.com/beta/lookup_uri?uri="
				+ StringUtils.urlEncode( v.toString() )
				+ "&lookup=Lookup";
		else
			throw new RippleException( "argument is neither a Literal nor a URI: " + v );

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
//				+ StringUtils.urlEncode( uri.toString() ) );
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

			synchronized( s_saxBuilder )
			{
				doc = s_saxBuilder.build( response );
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
					continue;
				}

				sink.put( new RippleList(
					new RdfValue( resultUri ), stack ) );
			}
		}

		// Nonempty result set (using the beta format)
		if ( root.getName().equals( "records" ) )
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
					continue;
				}

				sink.put( new RippleList(
					new RdfValue( resultUri ), stack ) );
			}
		}

		else if ( root.getName().equals( "nil-classes" ) )
		{
			// Do nothing.
		}

		else
			throw new RippleException( "Unexpected Sindice response: " + root.getName() );
	}
}

// kate: tab-width 4
