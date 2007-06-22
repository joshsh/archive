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
import java.net.URLEncoder;
import java.net.URLConnection;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

import org.openrdf.model.URI;

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
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

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

		URLConnection urlConn;

		try
		{
			URL url = new URL( "http://www.sindice.com/lookup?uri="
				+ StringUtils.urlEncode( uri.toString() ) );
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

		String []mimeTypes = { "text/xml" };
		HttpUtils.prepareUrlConnectionForRequest( urlConn, mimeTypes );

		Document doc;

		try
		{
			urlConn.connect();

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
				sink.put( new RippleList(
					new RdfValue( mc.createUri( s ) ), stack ) );
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
