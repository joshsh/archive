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

public class RippleException extends Exception
{
	final static Logger logger = Logger.getLogger( RippleException.class );

	public RippleException( final Throwable cause )
	{
		super( cause );
	}

	public RippleException( final String msg )
	{
		super( msg );
	}

	public void logError()
	{
		String description;

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

		try
		{
			logger.error( description );
		}

		catch ( Throwable t )
		{
			System.err.println( "Failed to log a RippleException. A stack trace of the secondary error follows." );
			t.printStackTrace( System.err );
		}
	}
}

// kate: tab-width 4