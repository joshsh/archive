package wurfel;

import org.apache.log4j.PropertyConfigurator;

public class Wurfel
{
    public static void initialize()
    {
        PropertyConfigurator.configure(
             Wurfel.class.getResource( "log4j.properties" ) );
//System.out.println("log4j.properties: " + Wurfel.class.getResource( "log4j.properties" ).toString() );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
