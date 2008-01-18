/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.apache.log4j.Logger;

/**
 * A custom Exception.
 */
public class RippleException extends Exception
{
	private static final long serialVersionUID = 2498405641024203574L;
	private static final Logger LOGGER = Logger.getLogger( RippleException.class );

	public RippleException( final Throwable cause )
	{
		super( cause );
	}

	public RippleException( final String msg )
	{
		super( msg );
	}

	public void logError( final boolean includeStackTrace )
	{
	   	String description;

		if ( includeStackTrace )
		{
			if ( null == getCause() )
			{
				description = getMessage();
			}

			else
			{
				try
				{
					ByteArrayOutputStream os = new ByteArrayOutputStream();
					PrintStream ps = new PrintStream( os );
					getCause().printStackTrace( ps );
					description = os.toString();
					ps.close();
					os.close();
				}

				catch ( java.io.IOException e )
				{
					System.err.println( "Failed to create error message. A stack trace of the secondary error follows." );
					e.printStackTrace( System.err );
					return;
				}
			}
		}

		else
		{
			if ( null == getCause() )
			{
				description = getMessage();
			}

			else
			{
				description = getCause().getMessage();
			}
		}

		try
		{
			LOGGER.error( description );
		}

		catch ( Throwable t )
		{
			System.err.println( "Failed to log an exception. A stack trace of the secondary error follows." );
			t.printStackTrace( System.err );
		}
	}

	public void logError()
	{
		logError( true );
	}
}

// kate: tab-width 4
