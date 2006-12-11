package wurfel;

import org.openrdf.model.URI;
import org.openrdf.model.impl.URIImpl;

import java.net.URL;

import org.apache.log4j.PropertyConfigurator;

public class Wurfel
{
    private static final URL
        s_wurfelSchemaUrl = Wurfel.class.getResource( "schema/wurfel.rdf" ),
        s_wurfelTestUrl = Wurfel.class.getResource( "schema/wurfel-test.rdf" );

    private static boolean initialized = false;

    public static void initialize()
    {
        if ( !initialized )
        {
            PropertyConfigurator.configure(
                Wurfel.class.getResource( "log4j.properties" ) );

            initialized = true;
        }
    }

    public static URL schemaUrl()
    {
        return s_wurfelSchemaUrl;
    }

    public static URL testUrl()
    {
        return s_wurfelTestUrl;
    }

    ////////////////////////////////////////////////////////////////////////////

    public static URI getRdfUri( final String localName )
    {
        return new URIImpl( "http://www.w3.org/1999/02/22-rdf-syntax-ns#" + localName );
    }

    public static URI getRdfSchemaUri( final String localName )
    {
        return new URIImpl( "http://www.w3.org/2000/01/rdf-schema#" + localName );
    }

    public static URI getWurfelUri( final String localName )
    {
        return new URIImpl( "urn:net.dnsdojo.troika.wurfel#" + localName );
    }

    public static URI getWurfelTestUri( final String localName )
    {
        return new URIImpl( "urn:net.dnsdojo.troika.wurfel-test#" + localName );
    }

    public static URI getXmlSchemaUri( final String localName )
    {
        return new URIImpl( "http://www.w3.org/2001/XMLSchema#" + localName );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
