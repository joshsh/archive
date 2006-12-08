package wurfel;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.apache.log4j.Logger;

public class WurfelException extends Exception
{
    private final static Logger s_logger
        = Logger.getLogger( WurfelException.class );

    public WurfelException( Throwable t )
    {
        super();

        try
        {
            ByteArrayOutputStream os = new ByteArrayOutputStream();
            PrintStream ps = new PrintStream( os );
            t.printStackTrace( ps );
            s_logger.error( os.toString() );

            System.err.println( t.toString() );
        }

        catch ( Throwable secondary )
        {
            System.err.println( "Failed to log a WurfelException. A stack trace of the secondary error follows." );
            secondary.printStackTrace( System.err );
        }
    }

    public WurfelException( final String msg )
    {
        super( msg );

        try
        {
            s_logger.error( msg );

            System.err.println( msg );
        }

        catch ( Throwable secondary )
        {
            System.err.println( "Failed to log a WurfelException. A stack trace of the secondary error follows." );
            secondary.printStackTrace( System.err );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
