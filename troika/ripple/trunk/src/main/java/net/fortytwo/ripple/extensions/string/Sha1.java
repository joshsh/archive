package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Sha1 extends PrimitiveFunction
{
	private static final String
		encoding = "UTF-8",
		algorithm = "SHA";

	private static MessageDigest messageDigest = null;

	public Sha1()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 1;
	}

	// See: http://intertwingly.net/stories/2003/08/05/sha1demo.java
	private synchronized String encrypt( final String plaintext )
		throws RippleException
	{
		try
		{
			if ( null == messageDigest )
				messageDigest = MessageDigest.getInstance( algorithm );
		}

		catch( NoSuchAlgorithmException e )
		{
			throw new RippleException( e );
		}

		try
		{
			messageDigest.update( plaintext.getBytes( encoding ) );
		}

		catch( UnsupportedEncodingException e )
		{
			throw new RippleException( e );
		}

		try
		{
			byte digest[] = messageDigest.digest();

			String coded = "";

			for  ( byte b : digest )
			{
				String hex = Integer.toHexString( b );
				if ( hex.length() == 1 )
					hex = "0" + hex;
				hex = hex.substring( hex.length() - 2 );
				coded += hex;
			}

			return coded;
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		String a, result;

		a = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		result = encrypt( a );

		sink.put( new RippleList( mc.createValue( result ), stack ) );
	}
}

// kate: tab-width 4
