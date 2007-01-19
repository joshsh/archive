package wurfel;

import org.openrdf.model.URI;
import org.openrdf.model.impl.URIImpl;

import java.net.URL;

import java.util.Properties;
import java.util.Random;

import java.io.IOException;

import org.apache.log4j.PropertyConfigurator;

/*
import com.ontogon.doapamine;

@Project(
    name="DOAPExample",
    homepage="http://www.bar.com",
    old_homepage={},
    category="DOAP",
    created="2005-10-15",
    shortdesc="DOAP Annotation Example",
    description="An example of using DOAPamine Java annoations.",
    mailinglist="foo@bar.com",
    license={"http://www.gnu.org/copyleft/lesser.html"},
    download_page="http://www.foo.com/doap",
    download_mirror={},
    wiki="",
    bug_database="",
    screenshots={},
    programming_language="FooScript",
    os={},
    release=@Version(name="unstable", created="2005-10-15", revision="0.1", file-release="http://foo.com/foo.tar"),
    maintainer={},
    developer={
        @Person(
            name="John Doe",
            mbox="jd@bar.com",
            seeAlso="http://www.bar.com/people/jd/foaf.rdf")
        },
    documentor={},
    translator={},
    tester={},
    helper={},
    repository=@Repository(
        type=Repository.RepositoryType.CVS, 
        module="doap", 
        location="foo", 
        browse="bar", 
        anon_root="foobar")
    )
*/
public class Wurfel
{
    public enum ExpressionOrder
    {
        DIAGRAMMATIC        ( "diagrammatic" ),
        ANTIDIAGRAMMATIC    ( "antidiagrammatic" );

        private String name;
        private  ExpressionOrder( String n )
        {
            name = n;
        }

        public static ExpressionOrder find( final String name )
            throws WurfelException
        {
            for ( ExpressionOrder x : ExpressionOrder.values() )
                if ( x.name.equals( name ) )
                    return x;

            String msg = "unknown ExpressionOrder: '" + name + "'";
            throw new WurfelException( msg );
        }
    }

    public enum ExpressionAssociativity
    {
        LEFT                ( "left" ),
        RIGHT               ( "right" );

        private String name;
        private  ExpressionAssociativity( String n )
        {
            name = n;
        }

        public static ExpressionAssociativity find( final String name )
            throws WurfelException
        {
            for ( ExpressionAssociativity x : ExpressionAssociativity.values() )
                if ( x.name.equals( name ) )
                    return x;

            String msg = "unknown ExpressionAssociativity: '" + name + "'";
            throw new WurfelException( msg );
        }
    }

    private static ExpressionOrder expressionOrder;
    private static ExpressionAssociativity expressionAssociativity;

    public static ExpressionOrder getExpressionOrder()
    {
        return expressionOrder;
    }

    public static ExpressionAssociativity getExpressionAssociativity()
    {
        return expressionAssociativity;
    }

    ////////////////////////////////////////////////////////////////////////////

    private static final URL
        s_wurfelSchemaUrl = Wurfel.class.getResource( "wurfel.rdf" );

    private static boolean initialized = false;

    public static void initialize()
        throws WurfelException
    {
        if ( !initialized )
        {
            PropertyConfigurator.configure(
                Wurfel.class.getResource( "log4j.properties" ) );

            Properties props = new Properties();

            try
            {
                props.load( Wurfel.class.getResourceAsStream( "wurfel.properties" ) );
            }

            catch ( IOException e )
            {
                throw new WurfelException( "unable to load wurfel.properties" );
            }

            expressionOrder = ExpressionOrder.find(
                props.getProperty( "wurfel.cli.syntax.order" ) );
            expressionAssociativity = ExpressionAssociativity.find(
                props.getProperty( "wurfel.cli.syntax.associativity" ) );

            initialized = true;
        }
    }

    public static URL schemaUrl()
    {
        return s_wurfelSchemaUrl;
    }

    ////////////////////////////////////////////////////////////////////////////

    private static Random rn = new Random();

    private static int randomInt( int lo, int hi )
    {
        int n = hi - lo + 1;
        int i = rn.nextInt() % n;
        if (i < 0)
            i = -i;
        return lo + i;
    }

    public static URI createUri( final String s )
        throws WurfelException
    {
        try
        {
            return new URIImpl( s );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    public static URI createRandomUri()
        throws WurfelException
    {
        return createUri( "urn:random:" + randomInt( 0, Integer.MAX_VALUE ) );
    }

    public static URI getRdfUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://www.w3.org/1999/02/22-rdf-syntax-ns#" + localName );
    }

    public static URI getRdfSchemaUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://www.w3.org/2000/01/rdf-schema#" + localName );
    }

    public static URI getWurfelUri( final String localName )
        throws WurfelException
    {
        return createUri( "urn:net.dnsdojo.troika.wurfel#" + localName );
    }

    public static URI getWurfelTestUri( final String localName )
        throws WurfelException
    {
        return createUri( "urn:net.dnsdojo.troika.wurfel-test#" + localName );
    }

    public static URI getXmlSchemaUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://www.w3.org/2001/XMLSchema#" + localName );
    }

    public static URI getSwoogleUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#" + localName );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
