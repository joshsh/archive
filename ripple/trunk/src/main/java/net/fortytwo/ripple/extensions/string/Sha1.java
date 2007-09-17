/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.string;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.Sink;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Sha1 extends PrimitiveFunction
{
	private static final int ARITY = 1;

	private static final String
		ENCODING = "UTF-8",
		ALGORITHM = "SHA";

	private static MessageDigest messageDigest = null;

	public Sha1()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	// See: http://intertwingly.net/stories/2003/08/05/sha1demo.java
	private synchronized String encrypt( final String plaintext )
		throws RippleException
	{
		try
		{
			if ( null == messageDigest )
			{
				messageDigest = MessageDigest.getInstance( ALGORITHM );
			}
		}

		catch ( NoSuchAlgorithmException e )
		{
			throw new RippleException( e );
		}

		try
		{
			messageDigest.update( plaintext.getBytes( ENCODING ) );
		}

		catch ( UnsupportedEncodingException e )
		{
			throw new RippleException( e );
		}

		try
		{
			byte[] digest = messageDigest.digest();

			String coded = "";

			for  ( byte b : digest )
			{
				String hex = Integer.toHexString( b );
				if ( hex.length() == 1 )
				{
					hex = "0" + hex;
				}
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
						final Sink<RippleList> sink,
						final ModelConnection mc )
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
