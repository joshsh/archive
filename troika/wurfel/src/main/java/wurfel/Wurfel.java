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

    private static int s_treeViewDepth;

    private static boolean s_dereferenceByNamespace;

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

    public static int getTreeViewDepth()
    {
        return s_treeViewDepth;
    }

    public static boolean dereferenceByNamespace()
    {
        return s_dereferenceByNamespace;
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

    private static int getIntProperty( final Properties props,
                                       final String name,
                                       final int defaultValue )
        throws WurfelException
    {
        String s = props.getProperty( name );

        if ( null == s )
            return defaultValue;

        else
        {
            try
            {
                return ( new Integer( s ) ).intValue();
            }

            catch ( java.lang.NumberFormatException e )
            {
                throw new WurfelException( e );
            }
        }
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

            s_dereferenceByNamespace = getBooleanProperty( props, "wurfel.model.uri.dereferenceByNamespace", false );

            s_treeViewDepth = getIntProperty( props, "wurfel.cli.treeView.depth", 1 );
            if ( s_treeViewDepth < 0 )
                s_treeViewDepth = 0;

            initialized = true;
        }
    }

    public static URL schemaUrl()
    {
System.out.println( "schemaUrl: " + s_wurfelSchemaUrl );
        return s_wurfelSchemaUrl;
    }

    public static String getWurfelName()
    {
        return "Slinky";
    }

    public static String getWurfelVersion()
    {
        return "0.1";
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
