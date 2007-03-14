package net.fortytwo.ripple.model;

import net.fortytwo.ripple.UrlFactory;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

//import org.openrdf.OpenRDFException;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.GraphQueryResult;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.Repository;
import org.openrdf.rio.rdfxml.util.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.model.Namespace;
import org.openrdf.repository.RepositoryResult;

import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;
import java.util.Set;

import org.apache.log4j.Logger;

public class Model extends Observable
{
// FIXME
    private Resource singleContext;

    private Dereferencer dereferencer;
public Dereferencer getDereferencer()
{
    return dereferencer;
}

    private final static Logger s_logger = Logger.getLogger( Model.class );
//    private static final AdminListener s_adminListener
//        = new StdOutAdminListener();

    private final static boolean s_useInferencing = true;

    String name;
    Repository repository;

Hashtable<String, String> aliases;

    private ModelBridge bridge;
    public ModelBridge getBridge()
    {
        return bridge;
    }

    private Hashtable<URI, Function> specialFunctions;

    private UrlFactory createUrlFactory()
        throws RippleException
    {
        ModelConnection mc = new ModelConnection( this, "for createUrlFactory()" );

        String wurfelNs, wurfelMiscNs, wurfelTestNs, rplNewNs;

        try
        {
            wurfelNs = mc.createRippleUri( "" ).toString();
            wurfelMiscNs = mc.createRippleMiscUri( "" ).toString();
            wurfelTestNs = mc.createRippleTestUri( "" ).toString();
            rplNewNs = "http://fortytwo.net/2007/03/04/rpl-new#";
        }

        catch ( RippleException e )
        {
            mc.close();
            throw e;
        }

        mc.close();
//System.out.println( wurfelNs );
//System.out.println( wurfelMiscNs );
//System.out.println( wurfelTestNs );

        Hashtable<String, String> urlMap = new Hashtable<String, String>();
        urlMap.put( wurfelNs,
            net.fortytwo.ripple.Ripple.class.getResource( "net.fortytwo.ripple.rdf" ) + "#" );
        urlMap.put( wurfelTestNs,
            net.fortytwo.ripple.extensions.test.TestExtension.class.getResource(
                "wurfel-test.rdf" ) + "#" );
        urlMap.put( wurfelMiscNs,
            net.fortytwo.ripple.extensions.misc.MiscExtension.class.getResource(
                "wurfel-misc.rdf" ) + "#" );
        urlMap.put( rplNewNs,
            net.fortytwo.ripple.extensions.newstuff.NewExtension.class.getResource(
                "rpl-new.rdf" ) + "#" );

        return new UrlFactory( urlMap );
    }

    /**
     *  @param Repository  an initialized Repository
     */
    public Model( final Repository repository, final String name )
        throws RippleException
    {
        s_logger.debug( "Creating new Model '" + name + "'" );

        this.repository = repository;
        this.name = name;

aliases = new Hashtable<String, String>();

        dereferencer = new HttpUriDereferencer( createUrlFactory() );

        bridge = new ModelBridge();

        ModelConnection mc = new ModelConnection( this, "for Model constructor" );

        try
        {
            ( new net.fortytwo.ripple.extensions.test.TestExtension() ).load( mc );
            ( new net.fortytwo.ripple.extensions.misc.MiscExtension() ).load( mc );
            ( new net.fortytwo.ripple.extensions.newstuff.NewExtension() ).load( mc );
        }

        catch ( RippleException e )
        {
            mc.close();
            throw e;
        }

        mc.close();
    }

public Repository getRepository()
{
    return repository;
}
//private boolean namespacesDefined = false;

    public void writeTo( OutputStream out )
        throws RippleException
    {
        // Note: a comment by Jeen suggests that a new writer should be created
        //       for each use:
        //       http://www.openrdf.org/forum/mvnforum/viewthread?thread=785#3159
        RDFXMLWriter writer = new RDFXMLPrettyWriter( out );

        try
        {
            RepositoryConnection con = repository.getConnection();
            con.export( /*singleContext,*/ writer );
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public void saveAs( String fileName )
        throws RippleException
    {
        OutputStream out;

        try
        {
            out = new FileOutputStream( fileName );
        }

        catch ( FileNotFoundException e )
        {
            throw new RippleException( e );
        }

        writeTo( out );

        try
        {
            out.close();
        }

        catch ( IOException e )
        {
            throw new RippleException( e );
        }
    }

// TODO: this operation is a little counterintuitive, in that it does not apply primitive functions
    public Collection<Value> multiply( Value arg, Value func, ModelConnection mc )
        throws RippleException
    {
        if ( arg instanceof RippleValue )
            arg = bridge.getRdfEquivalentOf( (RippleValue) arg, mc );

        if ( arg instanceof URI )
        {
            try
            {
                dereferencer.dereference( (URI) arg, mc );
            }

            catch ( RippleException e )
            {
                // (soft fail)
s_logger.debug( "Failed to dereference URI: " + arg.toString() );
            }
        }

        Iterator<Value> resultIter = rdfMultiply( arg, func, mc.getRepositoryConnection() ).iterator();
        Collection<Value> result = new Container();
        while ( resultIter.hasNext() )
        {
            Value v = resultIter.next();
            RippleValue rv = bridge.getNativeEquivalentOf( v );
            result.add( ( null == rv ) ? v : rv );
        }

        return result;
    }

    ////////////////////////////////////////////////////////////////////////////

    // Note: this may be a very expensive operation (see Sesame API).
    public long countStatements()
        throws RippleException
    {
        long size;

        try
        {
            RepositoryConnection con = repository.getConnection();
            size = con.size();
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return size;
    }

    public void define( String name, String uri )
    {
        aliases.put( name, uri );
    }

    public void showNamespaces()
        throws RippleException
    {
        try
        {
            RepositoryConnection conn = repository.getConnection();

            RepositoryResult<Namespace> nsIter
                 = conn.getNamespaces();
            int maxlen = 0;
            while ( nsIter.hasNext() )
            {
                Namespace ns = nsIter.next();
                int len = ns.getPrefix().length();
                if ( len > maxlen )
                    maxlen = len;
            }
            nsIter.close();

            nsIter = conn.getNamespaces();
            while ( nsIter.hasNext() )
            {
                Namespace ns = nsIter.next();
                String prefix = ns.getPrefix();
                int len = prefix.length();
                System.out.print( prefix + ":" );
                for ( int i = 0; i < maxlen - len + 2; i++ )
                    System.out.print( " " );
                System.out.println( ns.getName() );
            }
            nsIter.close();

            conn.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Collection<Value> getContexts()
        throws RippleException
    {
        Container contexts = new Container();

        try
        {
            RepositoryConnection conn = repository.getConnection();

            RepositoryResult<Resource> contextIter
                 = conn.getContextIDs();
            while ( contextIter.hasNext() )
                contexts.add( contextIter.next() );
            contextIter.close();

            conn.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return contexts;
    }

    public Collection<Statement> graphQuery( final String queryStr )
        throws RippleException
    {
        Collection<Statement> statements = new ArrayList<Statement>();

        try
        {
            RepositoryConnection con = repository.getConnection();
            GraphQueryResult result = con.prepareGraphQuery(
                QueryLanguage.SERQL, queryStr ).evaluate();
//                QueryLanguage.SERQL, "CONSTRUCT * FROM {x} p {y}");

// TODO: can I expect the Statements to remain valid after the connection is closed?
            while ( result.hasNext() )
                statements.add( result.next() );

            result.close();
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return statements;
    }

    // TODO: this method is useless, because we're not even displaying the results
    public void query( final String queryStr )
        throws RippleException
    {
        try
        {
            RepositoryConnection con = repository.getConnection();
            TupleQueryResult result = con.prepareTupleQuery(
                QueryLanguage.SERQL, queryStr ).evaluate();
//                QueryLanguage.SERQL, "SELECT x, y FROM {x} p {y}");

            result.close();
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
     *  @return  an unordered set of results
     */
    private Set<Value> rdfMultiply( Value subject,
                                   Value predicate,
                                   RepositoryConnection conn )
        throws RippleException
    {
        Set<Value> objects = new HashSet<Value>();

        if ( subject instanceof Resource && predicate instanceof URI )
        {
            try
            {
                RepositoryResult<Statement> stmtIter
                    = conn.getStatements(
//                        (Resource) subject, (URI) predicate, null, context, includeInferred );
                        (Resource) subject, (URI) predicate, null, Ripple.useInference() );
                while ( stmtIter.hasNext() )
                    objects.add( stmtIter.next().getObject() );
                stmtIter.close();
            }

            catch ( Throwable t )
            {
                throw new RippleException( t );
            }
        }

        return objects;
    }

    ////////////////////////////////////////////////////////////////////////////

    public void touch()
    {
        setChanged();
        notifyObservers();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
