package wurfel;

import java.net.URL;

import org.apache.log4j.PropertyConfigurator;

public class Wurfel
{
    private static URL wurfelSchemaUrl = null;

    public static void initialize()
    {
        PropertyConfigurator.configure(
             Wurfel.class.getResource( "log4j.properties" ) );

        wurfelSchemaUrl = Wurfel.class.getResource( "schema/wurfel.rdf" );
    }

    public static URL schemaUrl()
    {
        return wurfelSchemaUrl;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
