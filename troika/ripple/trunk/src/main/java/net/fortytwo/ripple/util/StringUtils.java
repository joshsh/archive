package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

public class StringUtils
{
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
						seq = s.substring( i + 1, i + 5 );
						sb.append( toUnicodeChar( seq ) );
						i += 4;
						break;
					case 'U':
						seq = s.substring( i + 1, i + 9 );
						sb.append( toUnicodeChar( seq ) );
						i += 8;
						break;
					default:
						throw new RippleException( "bad escape sequence: \\" + s.charAt( i + 1 ) );
				}
			}

			else
				sb.append( c );
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
						seq = s.substring( i + 1, i + 5 );
						sb.append( toUnicodeChar( seq ) );
						i += 4;
						break;
					case 'U':
						seq = s.substring( i + 1, i + 9 );
						sb.append( toUnicodeChar( seq ) );
						i += 8;
						break;
					default:
						throw new RippleException( "bad escape sequence: \\" + s.charAt( i + 1 ) );
				}
			}

			else
				sb.append( c );
		}

		return sb.toString();
	}

	// See:
	//    http://www.koders.com/java/fid97184BECA1A7DCCD2EDA6D243477157EB453294C.aspx
	final static String safeURLCharacters
		= "@*-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";

	public static String urlEncode( final String url )
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

	////////////////////////////////////////////////////////////////////////////

	private static char toUnicodeChar( final String unicode )
		throws RippleException
	{
		try
		{
			return (char) Integer.parseInt( unicode.toString(), 16 );
		}

		catch ( NumberFormatException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
