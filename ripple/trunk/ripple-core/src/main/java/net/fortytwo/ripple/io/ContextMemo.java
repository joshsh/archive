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
	private static final String STATUS = "status";
	private static final String TIMESTAMP = "timestamp";
	private static final String MEDIATYPE = "mediaType";

	// Use XMLSchema-style time stamps, without time zone info, accurate to
	// the nearest second.
	private static final SimpleDateFormat dateFormat = new SimpleDateFormat( "yyyy'-'MM'-'dd'T'HH':'mm':'ss" );

	private Rdfizer.Outcome status = null;
	private Date timestamp = null;
	private MediaType mediaType = null;

	public ContextMemo()
	{
		this( Rdfizer.Outcome.Undetermined );
	}

	public ContextMemo( final Rdfizer.Outcome status )
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
				this.status = Rdfizer.Outcome.valueOf( value );
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

	public Rdfizer.Outcome getStatus()
	{
		return status;
	}

	public void setStatus( final Rdfizer.Outcome status )
	{
		this.status = status;
	}

	public void setMediaType( final MediaType mt )
	{
		this.mediaType = mt;
	}
}
