package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import org.restlet.data.MediaType;

import java.text.FieldPosition;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Author: josh
 * Date: Jan 18, 2008
 * Time: 3:47:59 PM
 */
public class ContextMemo
{
	public static enum Status
	{
		Undetermined,       // to be used only when a memo is created
		Success,            // normal outcome
		Timeout,            // network timeout
		InvalidUri,         // bad URI
		ParseError,         // a document was received, but failed to parse
		ClientError,        // 4xx HTTP error
		ServerError,        // 5xx HTTP error
		BadUriScheme,       // no suitable URI dereferencer was found
		BadMediaType,       // no suitable rdfizer was found
		DereferencerError,  // TODO: break this down into more specific conditions
		RdfizerError,       // TODO: break this down into more specific conditions
		Ignored,            // don't bother dereferencing these URIs
		Failure             // all other error conditions
	}

	private static final String STATUS = "status";
	private static final String TIMESTAMP = "timestamp";
	private static final String MEDIATYPE = "mediaType";

	// Use XMLSchema-style time stamps, without time zone info, accurate to
	// the nearest second.
	private static final SimpleDateFormat dateFormat = new SimpleDateFormat( "yyyy'-'MM'-'dd'T'HH':'mm':'ss" );

	private Status status = null;
	private Date timestamp = null;
	private MediaType mediaType = null;

	public ContextMemo()
	{
		this( Status.Undetermined );
	}

	public ContextMemo( final Status status )
	{
		this.status = status;
		this.timestamp = new Date();
	}

	// TODO: parse error handling
	public ContextMemo( final String s ) throws RippleException
	{
	    String[] props = s.split( ";" );
		for ( int i = 0; i < props.length; i++ )
		{
			String prop = props[i];
			int eq = prop.indexOf( '=' );
			String name = prop.substring( 0, eq ).trim();
			String value = prop.substring( eq + 1 ).trim();

			if ( name.equals( STATUS ) )
			{
				this.status = Status.valueOf( value );
			}

			else if ( name.equals( TIMESTAMP ) )
			{
				this.timestamp = dateFormat.parse( value, new ParsePosition( 0 ) );
			}

			else if ( name.equals( MEDIATYPE ) )
			{
				// TODO: is it reasonable to *create* a new media type here?
				this.mediaType = new MediaType( value );
			}
		}
	}

	public String toString()
	{
		StringBuffer sb = new StringBuffer();

		sb.append( STATUS ).append( "=" ).append( status );

		if ( null != timestamp )
		{
			sb.append( "; " ).append( TIMESTAMP ).append( "=" );
			dateFormat.format( timestamp, sb, new FieldPosition( 0 ) );
		}

		if ( null != mediaType )
		{
			sb.append( "; " ).append( MEDIATYPE ).append( "=" ).append( mediaType.toString() );
		}

		return sb.toString();
	}

	public Status getStatus()
	{
		return status;
	}

	public void setStatus( final Status status )
	{
		this.status = status;
	}

	public void setMediaType( final MediaType mt )
	{
		this.mediaType = mt;
	}
}
