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

public class PingTheSemanticWeb extends PrimitiveFunction
{
	static SAXBuilder saxBuilder = null;
	static void initialize()
	{
		saxBuilder = new SAXBuilder( true );
		saxBuilder.setReuseParser( true );

		String schemaLocation = PingTheSemanticWeb.class.getResource( "pingTheSemanticWeb.xsd" ).toString();
		saxBuilder.setFeature(
			"http://apache.org/xml/features/validation/schema", true );
		saxBuilder.setProperty( "http://apache.org/xml/properties/schema/"
			+ "external-noNamespaceSchemaLocation", schemaLocation );
	}

	public PingTheSemanticWeb()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 2;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		if ( null == saxBuilder )
			initialize();

		String type;
		int maxResults;

		type = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();
		maxResults = mc.intValue( stack.getFirst() );
		stack = stack.getRest();

		URLConnection urlConn = HttpUtils.openConnection(
			"http://pingthesemanticweb.com/export/?serialization=xml&ns=&domain=&timeframe=any_time&type=" + type + "&nbresults=" + maxResults );

		String[] mimeTypes = { "text/xml" };
		HttpUtils.prepareUrlConnectionForRequest( urlConn, mimeTypes );
		HttpUtils.connect( urlConn );

		Document doc;

		try
		{
			InputStream response = urlConn.getInputStream();

			synchronized( saxBuilder )
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
		Iterator<Element> childIter = root.getChildren().iterator();
		while ( childIter.hasNext() )
		{
			Element child = childIter.next();
			String s = child.getAttributeValue( "url" );
			sink.put( new RippleList(
				new RdfValue( mc.createUri( s ) ), stack ) );
		}
	}
}

// kate: tab-width 4
