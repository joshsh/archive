/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.HttpUtils;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import java.net.URL;
import java.net.URLConnection;

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

		String uriStr = mc.uriValue( stack.getFirst() ).toString();
		stack = stack.getRest();

		URLConnection urlConn;

		try
		{
			URL url = new URL( uriStr );
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

		HttpUtils.prepareUrlConnectionForTextRequest( urlConn );
		HttpUtils.connect( urlConn );

		try
		{
			InputStream response = urlConn.getInputStream();

			BufferedReader br = new BufferedReader(
				new InputStreamReader( response ) );
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

			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		sink.put( mc.list( mc.value( result ), stack ) );
	}
}

// kate: tab-width 4
