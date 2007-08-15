package net.fortytwo.ripple;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.apache.log4j.Logger;

public class RippleException extends Exception
{
	final static Logger logger = Logger.getLogger( RippleException.class );

	public RippleException( Throwable t )
	{
		super( t.toString() );

		try
		{
			ByteArrayOutputStream os = new ByteArrayOutputStream();
			PrintStream ps = new PrintStream( os );
			t.printStackTrace( ps );
			logger.error( os.toString() );

			ps.close();
			os.close();
		}

		catch ( Throwable secondary )
		{
			System.err.println( "Failed to log a RippleException. A stack trace of the secondary error follows." );
			secondary.printStackTrace( System.err );
		}
	}

	public RippleException( final String msg )
	{
		super( msg );

printStackTrace( System.err );
// System.err.println( "" );
		try
		{
			logger.error( msg );
		}

		catch ( Throwable secondary )
		{
			System.err.println( "Failed to log a RippleException. A stack trace of the secondary error follows." );
			secondary.printStackTrace( System.err );
		}
	}

	public void log()
	{
// ...
	}
}

// kate: tab-width 4
