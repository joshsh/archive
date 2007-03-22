package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.net.URL;
import java.net.URLConnection;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class UrlTarget extends PrimitiveFunction
{
    public UrlTarget( RdfValue v, ModelConnection mc )
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

        try
        {
            URL url = new URL( a );
            URLConnection connection = url.openConnection();
            connection.connect();

            InputStream response = connection.getInputStream();

            BufferedReader br = new BufferedReader(
                new InputStreamReader( response ) );
            StringBuffer sb = new StringBuffer();
            String nextLine = "";
            while ( ( nextLine = br.readLine() ) != null )
                 sb.append(nextLine);
            result = sb.toString();

            response.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        sink.put( new RippleList( mc.createValue( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
