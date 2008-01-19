package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

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

	// Use XMLSchema-style time stamps, without time zone info, accurate to
	// the nearest second.
	private static final SimpleDateFormat dateFormat = new SimpleDateFormat( "yyyy'-'MM'-'dd'T'HH':'mm':'ss" );

	private Rdfizer.Outcome outcome = null;
	private Date when = null;

	public ContextMemo( final Rdfizer.Outcome outcome )
	{
		this.outcome = outcome;
		this.when = new Date();
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
				this.outcome = Rdfizer.Outcome.valueOf( value );
			}

			else if ( name.equals( TIMESTAMP ) )
			{
				this.when = dateFormat.parse( value, new ParsePosition( 0 ) );
			}
		}
	}

	public String toString()
	{
		StringBuffer sb = new StringBuffer();

		sb.append( STATUS ).append( "=" ).append( outcome );

		if ( null != when )
		{
			sb.append( "; " ).append( TIMESTAMP ).append( "=" );
			dateFormat.format( when, sb, new FieldPosition( 0 ) );
		}

		return sb.toString();
	}

	public Rdfizer.Outcome getOutcome()
	{
		return outcome;
	}
}
