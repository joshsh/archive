package net.fortytwo.ripple.extensions.services;

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

import java.util.Iterator;
import java.util.List;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.xpath.XPath;

import org.openrdf.model.URI;

public class Uriqr extends PrimitiveFunction
{
	static SAXBuilder s_saxBuilder = null;
	static XPath resultPath = null;
	static void initialize()
		throws RippleException
	{
			s_saxBuilder = new SAXBuilder( true );
			s_saxBuilder.setReuseParser( true );

/*
			String schemaLocation = Uriqr.class.getResource( "uriqr.xsd" ).toString();
			s_saxBuilder.setFeature(
				"http://apache.org/xml/features/validation/schema", true );
			s_saxBuilder.setProperty( "http://apache.org/xml/properties/schema/"
				+ "external-noNamespaceSchemaLocation", schemaLocation );
*/

			try
			{
				resultPath = XPath.newInstance( "html/body/div[@id='content']/p/strong" );
			}

			catch ( org.jdom.JDOMException e )
			{
				throw new RippleException( e );
			}
	}

	public Uriqr()
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
		if ( null == s_saxBuilder )
			initialize();

		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		String urlStr = "http://dev.uriqr.com/search.php?query="
			+ StringUtils.urlEncode( s );

		URLConnection urlConn;

		try
		{
			URL url = new URL( urlStr );
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

		String []mimeTypes = { "application/xhtml+xml", "application/xml", "text/xml" };
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

		List<Object> results;

		try
		{
			results = resultPath.selectNodes( doc );
		}

		catch ( org.jdom.JDOMException e )
		{
			throw new RippleException( e );
		}

		for ( Iterator<Object> iter = results.iterator(); iter.hasNext(); )
		{
			Object r = iter.next();
			if ( r instanceof Element )
			{
				String text = ( ( Element ) r ).getText();
				URI resultUri = mc.createUri( text );

				sink.put( new RippleList(
					new RdfValue( resultUri ), stack ) );
			}

			throw new RippleException( "unexpected result format" );
		}
	}
}

// kate: tab-width 4
