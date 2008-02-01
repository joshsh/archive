/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.services;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.StringUtils;

import java.io.InputStream;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;

import org.openrdf.model.URI;

/**
 * A primitive which consumes a literal value and produces a number of resources
 * which link to that value, according to the Uriqr service.
 */
public class Uriqr extends PrimitiveFunction
{
	private static final int ARITY = 1;

	private static SAXBuilder saxBuilder = null;

	private static void initialize() throws RippleException
	{
		saxBuilder = new SAXBuilder( true );
		saxBuilder.setReuseParser( true );

/*
			String schemaLocation = Uriqr.class.getResource( "uriqr.xsd" ).toString();
			saxBuilder.setFeature(
				"http://apache.org/xml/features/validation/schema", true );
			saxBuilder.setProperty( "http://apache.org/xml/properties/schema/"
				+ "external-noNamespaceSchemaLocation", schemaLocation );
*/

/* hefty Jaxen dependency removed
		try
		{
//XPath.setXPathClass( org.jdom.xpath.JaxenXPath.class );
			resultPath = XPath.newInstance( "html/body/div[@id='content']/p/strong" );
		}

		catch ( org.jdom.JDOMException e )
		{
			throw new RippleException( e );
		}
*/
	}

	public Uriqr()
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
						final Context context )
		throws RippleException
	{
		final ModelConnection mc = context.getModelConnection();

		if ( null == saxBuilder )
		{
			initialize();
		}

		String s;

		s = mc.toString( stack.getFirst() );
		stack = stack.getRest();

		String urlStr = "http://dev.uriqr.com/search.php?query="
			+ StringUtils.urlEncode( s );

		String []mimeTypes = { "application/xhtml+xml", "application/xml", "text/xml" };
		HttpMethod method = HttpUtils.createGetMethod( urlStr );
		HttpUtils.setAcceptHeader( method, mimeTypes );
//		uc.setConnectTimeout( (int) Ripple.urlConnectTimeout() );
		HttpUtils.registerMethod( method );
	
		HttpClient client = HttpUtils.createClient();
		
		InputStream body;
		
		try
		{
			client.executeMethod( method );
	        body = method.getResponseBodyAsStream();
		}
		
		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		Document doc;

		try
		{
			synchronized ( saxBuilder )
			{
				doc = saxBuilder.build( body );
			}

			body.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		catch ( org.jdom.JDOMException e )
		{
			throw new RippleException( e );
		}
		
		try
		{
			method.releaseConnection();
		}
		
		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		List results = selectNodes( doc );

		for ( Iterator iter = results.iterator(); iter.hasNext(); )
		{
			Object r = iter.next();
			if ( r instanceof Element )
			{
				String text = ( (Element) r ).getText();
				URI resultUri = mc.createUri( text );

				sink.put( mc.list(
					new RdfValue( resultUri ), stack ) );
			}

			throw new RippleException( "unexpected result format" );
		}
	}

	List selectNodes( final Document doc ) throws RippleException
	{
/*
		try
		{
			return resultPath.selectNodes( doc );
		}

		catch ( org.jdom.JDOMException e )
		{
			throw new RippleException( e );
		}
*/
		List results = new LinkedList();
		Element html = doc.getRootElement();
		Iterator<Element> divIter = html.getChildren( "div" ).iterator();
		while ( divIter.hasNext() )
		{
			Element div = divIter.next();
			String id = div.getAttributeValue( "id" );
			if ( id != null && id.equals( "content" ) )
			{
				Iterator<Element> pIter = div.getChildren( "p" ).iterator();
				while ( pIter.hasNext() )
				{
					Element p = pIter.next();
					Iterator<Element> strongIter = p.getChildren( "strong" ).iterator();
					while ( strongIter.hasNext() )
					{
						results.add( strongIter.next() );
					}
				}
			}
		}
		return results;
	}
}

// kate: tab-width 4
