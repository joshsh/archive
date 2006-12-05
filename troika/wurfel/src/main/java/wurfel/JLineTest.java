package wurfel;

import wurfel.cli.Interpreter;

import org.apache.log4j.PropertyConfigurator;

public class JLineTest
{
    public static void main( final String [] args )
    {
        Wurfel.initialize();

        try
        {
            Context context = new Context( "anonymousContext" );
            Interpreter r = new Interpreter( context );
            r.run();
        }

        catch ( Throwable t )
        {
            System.out.println( t.toString() );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
