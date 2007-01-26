package wurfel;

import java.net.URL;

import java.util.Properties;

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

    ////////////////////////////////////////////////////////////////////////////

    private static final URL
        s_wurfelSchemaUrl = Wurfel.class.getResource( "wurfel.rdf" );

    private static boolean initialized = false;

    private static ExpressionOrder expressionOrder;
    private static ExpressionAssociativity expressionAssociativity;

    private static String s_jLineDebugOutput;

    private static boolean
        s_useInference,
        s_enforceImplicitProvenance;

    ////////////////////////////////////////////////////////////////////////////

    public static ExpressionOrder getExpressionOrder()
    {
        return expressionOrder;
    }

    public static ExpressionAssociativity getExpressionAssociativity()
    {
        return expressionAssociativity;
    }

    public static String getJLineDebugOutput()
    {
        return s_jLineDebugOutput;
    }

    public static boolean useInference()
    {
        return s_useInference;
    }

    public static boolean enforceImplicitProvenance()
    {
        return s_enforceImplicitProvenance;
    }

    ////////////////////////////////////////////////////////////////////////////

    private static boolean getBooleanProperty( final Properties props,
                                               final String name,
                                               boolean defaultValue )
    {
        String s = props.getProperty( name );

        return ( null == s )
            ? defaultValue
            : s.equals( "true" );
    }

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

            s_jLineDebugOutput = props.getProperty( "wurfel.cli.jline.debugOutput" );

            s_useInference = getBooleanProperty( props, "wurfel.model.rdf.useInference", false );
            s_enforceImplicitProvenance = getBooleanProperty( props, "wurfel.model.rdf.enforceImplicitProvenance", true );

            initialized = true;
        }
    }

    public static URL schemaUrl()
    {
System.out.println( "schemaUrl: " + s_wurfelSchemaUrl );
        return s_wurfelSchemaUrl;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
