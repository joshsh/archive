/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.net.URLEncoder;

import net.fortytwo.ripple.RippleException;

public final class StringUtils
{
	// See: http://www.koders.com/java/fid97184BECA1A7DCCD2EDA6D243477157EB453294C.aspx
	private static final String SAFE_URL_CHARACTERS
		= "@*-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

	private static final int
		FOUR = 4,
		EIGHT = 8,
		SIXTEEN = 16;

	private StringUtils()
	{
	}

	// Note: extended characters are not escaped for printing.
	public static String escapeString( final String s )
	{
		StringBuilder sb = new StringBuilder();
		for ( int i = 0; i < s.length(); i++ )
		{
			char c = s.charAt( i );
			switch ( c )
			{
				case '\\':
					sb.append( "\\\\" );
					break;
				case '\"':
					sb.append( "\\\"" );
					break;
				case '\t':
					sb.append( "\\t" );
					break;
				case '\n':
					sb.append( "\\n" );
					break;
				case '\r':
					sb.append( "\\r" );
					break;
				default:
					sb.append( c );
			}
		}

		return sb.toString();
	}

	// Note: extended characters are not escaped for printing.
	public static String escapeUriString( final String s )
	{
		StringBuilder sb = new StringBuilder();
		for ( int i = 0; i < s.length(); i++ )
		{
			char c = s.charAt( i );
			switch ( c )
			{
				case '\\':
					sb.append( "\\\\" );
					break;
				case '>':
					sb.append( "\\>" );
					break;
				default:
					sb.append( c );
			}
		}

		return sb.toString();
	}

	// Note: assumes a properly formatted (escaped) String argument.
	public static String unescapeString( final String s )
		throws RippleException
	{
		StringBuilder sb = new StringBuilder();
		String seq;

		for ( int i = 0; i < s.length(); i++ )
		{
			char c = s.charAt( i );

			if ( '\\' == c )
			{
				i++;

				switch ( s.charAt( i ) )
				{
					case '\\':
						sb.append( '\\' );
						break;
					case '\"':
						sb.append( '\"' );
						break;
					case 't':
						sb.append( '\t' );
						break;
					case 'n':
						sb.append( '\n' );
						break;
					case 'r':
						sb.append( '\r' );
						break;
					case 'u':
						seq = s.substring( i + 1, i + FOUR + 1 );
						sb.append( toUnicodeChar( seq ) );
						i += FOUR;
						break;
					case 'U':
						seq = s.substring( i + 1, i + EIGHT + 1 );
						sb.append( toUnicodeChar( seq ) );
						i += EIGHT;
						break;
					default:
						throw new RippleException( "bad escape sequence: \\" + s.charAt( i + 1 ) );
				}
			}

			else
			{
				sb.append( c );
			}
		}

		return sb.toString();
	}

	// Note: assumes a properly formatted (escaped) String argument.
	public static String unescapeUriString( final String s )
		throws RippleException
	{
		StringBuilder sb = new StringBuilder();
		String seq;

		for ( int i = 0; i < s.length(); i++ )
		{
			char c = s.charAt( i );

			if ( '\\' == c )
			{
				i++;

				switch ( s.charAt( i ) )
				{
					case '\\':
						sb.append( '\\' );
						break;
					case '>':
						sb.append( '>' );
						break;
					case 'u':
						seq = s.substring( i + 1, i + FOUR + 1 );
						sb.append( toUnicodeChar( seq ) );
						i += FOUR;
						break;
					case 'U':
						seq = s.substring( i + 1, i + EIGHT + 1 );
						sb.append( toUnicodeChar( seq ) );
						i += EIGHT;
						break;
					default:
						throw new RippleException( "bad escape sequence: \\" + s.charAt( i + 1 ) );
				}
			}

			else
			{
				sb.append( c );
			}
		}

		return sb.toString();
	}

	/**
	 *  @param s  a string to encode
	 *  @return  percent-encoded (per RFC 3986) version of the string
	 */
	public static String percentEncode( final String s )
	{
		StringBuffer enc = new StringBuffer( s.length() );
		for ( int i = 0; i < s.length(); ++i )
		{
			char c = s.charAt( i );

			if (SAFE_URL_CHARACTERS.indexOf( c ) >= 0)
			{
				enc.append( c );
			}

			else
			{
				// Just keep lsb like:
				// http://java.sun.com/j2se/1.3/docs/api/java/net/URLEncoder.html
				c = (char) ( c & '\u00ff' );
				if ( c < SIXTEEN )
				{
					enc.append( "%0" );
				}
				else
				{
					enc.append( "%" );
				}

				enc.append( Integer.toHexString( c ) );
			}
		}
		return enc.toString();
	}

	/**
	 *  @param s  a string to encode
	 *  @return  application/x-www-form-urlencoded version of the string
	 */
 	public static String urlEncode( final String s )
		throws RippleException
	{
		try
		{
			return URLEncoder.encode( s, "UTF-8" );
		}

		catch ( java.io.UnsupportedEncodingException e )
		{
			throw new RippleException( e );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private static char toUnicodeChar( final String unicode )
		throws RippleException
	{
		try
		{
			return (char) Integer.parseInt( unicode.toString(), SIXTEEN );
		}

		catch ( NumberFormatException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
