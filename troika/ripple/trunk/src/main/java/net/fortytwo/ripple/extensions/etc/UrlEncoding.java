package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

//import java.net.URLEncoder;

public class UrlEncoding extends PrimitiveFunction
{
	public UrlEncoding( RdfValue v, ModelConnection mc )
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

		result = encode( a );
/*
		try
		{
			result = URLEncoder.encode( a, "UTF-8" );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
*/

		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}

	// Below this line is from:
	//    http://www.koders.com/java/fid97184BECA1A7DCCD2EDA6D243477157EB453294C.aspx

	final static String safeURLCharacters
		= "@*-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

	private final static String encode( String url )
	{
		StringBuffer enc = new StringBuffer( url.length() );
		for ( int i = 0; i < url.length(); ++i )
		{
			char c = url.charAt(i);

			if (safeURLCharacters.indexOf(c) >= 0)
			{
				enc.append(c);
			}

			else
			{
				// Just keep lsb like:
				// http://java.sun.com/j2se/1.3/docs/api/java/net/URLEncoder.html
				c = (char) (c & '\u00ff');
				if (c < 16)
					enc.append("%0");
				else
					enc.append("%");

				enc.append( Integer.toHexString( c ) );
			}
		}
		return enc.toString();
	}
}

// kate: tab-width 4
