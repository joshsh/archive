package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.HttpUtils;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import java.net.URL;
import java.net.URLConnection;

public class Get extends PrimitiveFunction
{
	public Get( RdfValue v, ModelConnection mc )
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
		String a, result;

		a = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		URLConnection urlConn;

		try
		{
			URL url = new URL( a );
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
			while ( ( nextLine = br.readLine() ) != null )
				sb.append( nextLine );
			result = sb.toString();

			response.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}
}

// kate: tab-width 4
