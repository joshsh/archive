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
        try
        {
            ByteArrayOutputStream os = new ByteArrayOutputStream();
            PrintStream ps = new PrintStream( os );
            t.printStackTrace( ps );
            s_logger.error( os.toString() );

            System.err.println( t.toString() );
        }

        catch ( Throwable t2 )
        {
            System.err.println( "Failed to log an error passed to WurfelException. A stack trace of the secondary error follows." );
            t.printStackTrace( System.err );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
