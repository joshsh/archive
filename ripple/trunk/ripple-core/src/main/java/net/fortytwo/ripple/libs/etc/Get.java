/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.etc;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.Sink;

import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;

/**
 * A primitive which consumes an information resource, issues a GET request for
 * the resource, then produces the retrieved data as a string.
 */
public class Get extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Get()
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
		String result;

		String uriStr = mc.toUri( stack.getFirst() ).toString();
		stack = stack.getRest();

		HttpMethod method = HttpUtils.createGetMethod( uriStr );
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

		try
		{
			BufferedReader br = new BufferedReader(
				new InputStreamReader( body ) );
			StringBuffer sb = new StringBuffer();
			String nextLine = "";
			boolean first = true;
			while ( ( nextLine = br.readLine() ) != null )
			{
				if ( first )
				{
					first = false;
				}

				else
				{
					sb.append( '\n' );
				}

				sb.append( nextLine );
			}
			result = sb.toString();

			body.close();
		}

		catch ( java.io.IOException e )
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
		
		sink.put( mc.list( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
