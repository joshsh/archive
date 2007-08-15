package net.fortytwo.ripple;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.apache.log4j.Logger;

public class RippleException extends Exception
{
    private final static Logger s_logger
        = Logger.getLogger( RippleException.class );

    private String msg;

    protected static int
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4;

/*
    protected RippleException( Throwable t, int logLevel )
    {
        try
        {
            boolean printTrace = false;

            switch ( logLevel )
            {
                case DEBUG:

                case INFO:

                case WARN:

                case ERROR:
                    printTrace = true;
            }

            if ( printTrace )
            {
                ByteArrayOutputStream os = new ByteArrayOutputStream();
                PrintStream ps = new PrintStream( os );
                t.printStackTrace( ps );
                s_logger.error( os.toString() );

                System.err.println( t.toString() );
            }
        }

        catch ( Throwable secondary )
        {
            System.err.println( "Failed to log a RippleException. A stack trace of the secondary error follows." );
            secondary.printStackTrace( System.err );
        }
    }
*/

    public RippleException( Throwable t )
    {
        super();

        try
        {
            ByteArrayOutputStream os = new ByteArrayOutputStream();
            PrintStream ps = new PrintStream( os );
            t.printStackTrace( ps );
            s_logger.error( os.toString() );

            msg = t.toString();
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

        try
        {
            s_logger.error( msg );
        }

        catch ( Throwable secondary )
        {
            System.err.println( "Failed to log a RippleException. A stack trace of the secondary error follows." );
            secondary.printStackTrace( System.err );
        }
    }

    public String getMessage()
    {
        return msg;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
