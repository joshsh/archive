/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.HTTPUtils;
import net.fortytwo.ripple.flow.Sink;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpMethod;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * A primitive which consumes an information resource, issues a GET request for
 * the resource, then produces the retrieved data as a string.
 */
public class Get extends PrimitiveStackMapping
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink
	)
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

		String result;

		String uriStr = mc.toUri( stack.getFirst() ).toString();
		stack = stack.getRest();

		HttpMethod method = HTTPUtils.createGetMethod( uriStr );
		HTTPUtils.registerMethod( method );
		
		HttpClient client = HTTPUtils.createClient();
		
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
		
		sink.put( arg.with( stack.push( mc.value( result ) ) ) );
	}
}

// kate: tab-width 4
