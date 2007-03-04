package net.fortytwo.ripple.model;

import wurfel.UrlFactory;
import wurfel.Wurfel;
import wurfel.WurfelException;

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

    private Hashtable<URI, Function> specialFunctions;

    public void addSpecialFunction( Function f )
    {
        specialFunctions.put( f.getUri(), f );
    }

    private UrlFactory createUrlFactory()
        throws WurfelException
    {
        ModelConnection mc = new ModelConnection( this, "for createUrlFactory()" );

        String wurfelNs, wurfelMiscNs, wurfelTestNs, rplNewNs;

        try
        {
            wurfelNs = mc.createWurfelUri( "" ).toString();
            wurfelMiscNs = mc.createWurfelMiscUri( "" ).toString();
            wurfelTestNs = mc.createWurfelTestUri( "" ).toString();
            rplNewNs = "http://fortytwo.net/2007/03/04/rpl-new#";
        }

        catch ( WurfelException e )
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
            wurfel.Wurfel.class.getResource( "wurfel.rdf" ) + "#" );
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
        throws WurfelException
    {
        s_logger.debug( "Creating new Model '" + name + "'" );

        this.repository = repository;
        this.name = name;

aliases = new Hashtable<String, String>();

        dereferencer = new HttpUriDereferencer( createUrlFactory() );

        specialFunctions = new Hashtable<URI, Function>();

        ModelConnection mc = new ModelConnection( this, "for Model constructor" );

        try
        {
//            importModel( Wurfel.schemaUrl(), mc.createUri( "urn:wurfel" ), mc );

            ( new net.fortytwo.ripple.extensions.test.TestExtension() ).load( mc );
            ( new net.fortytwo.ripple.extensions.misc.MiscExtension() ).load( mc );
            ( new net.fortytwo.ripple.extensions.newstuff.NewExtension() ).load( mc );
        }

        catch ( WurfelException e )
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

    private void extractRDF( OutputStream out )
        throws WurfelException
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
            throw new WurfelException( t );
        }
    }

    public void saveAs( String fileName )
        throws WurfelException
    {
        OutputStream out;

        try
        {
            out = new FileOutputStream( fileName );
        }

        catch ( FileNotFoundException e )
        {
            throw new WurfelException( e );
        }

        extractRDF( out );

        try
        {
            out.close();
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }
    }

    private boolean isApply( Value v )
    {
        return v instanceof Apply;
    }

    public Value translateFromGraph( Value v )
    {
        if ( v instanceof URI )
        {
            Function f = specialFunctions.get( (URI) v );

            if ( null != f )
                return f;
            else
                return v;
        }

        else
            return v;
    }

// FIXME: will return null if the Function is an Apply
    private Value translateToGraph( Value v )
    {
        if ( v instanceof Function )
            return ( (Function) v ).getUri();
        else
            return v;
    }

// TODO: this operation is a little counterintuitive, in that it does not apply primitive functions
    public Set<Value> multiply( Value arg, Value func, ModelConnection mc )
        throws WurfelException
    {
        arg = translateToGraph( arg );

        if ( arg instanceof URI )
        {
            try
            {
                dereferencer.dereference( (URI) arg, mc );
            }

            catch ( WurfelException e )
            {
                // (soft fail)
s_logger.debug( "Failed to dereference URI: " + arg.toString() );
            }
        }

        Iterator<Value> resultIter = rdfMultiply( arg, func, mc.getRepositoryConnection() ).iterator();
        Set<Value> result = new Container();
        while ( resultIter.hasNext() )
        {
            Value v = resultIter.next();
            result.add( translateFromGraph( v ) );
        }

        return result;
    }

    ////////////////////////////////////////////////////////////////////////////

    // Note: this may be a very expensive operation (see Sesame API).
    public long countStatements()
        throws WurfelException
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
            throw new WurfelException( t );
        }

        return size;
    }

    public void define( String name, String uri )
    {
        aliases.put( name, uri );
    }

    public void showNamespaces()
        throws WurfelException
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
            throw new WurfelException( t );
        }
    }

    public Collection<Value> getContexts()
        throws WurfelException
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
            throw new WurfelException( t );
        }

        return contexts;
    }

    public Collection<Statement> graphQuery( final String queryStr )
        throws WurfelException
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
            throw new WurfelException( t );
        }

        return statements;
    }

    // TODO: this method is useless, because we're not even displaying the results
    public void query( final String queryStr )
        throws WurfelException
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
            throw new WurfelException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    /**
     *  @return  an unordered set of results
     */
    private Set<Value> rdfMultiply( Value subject,
                                   Value predicate,
                                   RepositoryConnection conn )
        throws WurfelException
    {
        Set<Value> objects = new HashSet<Value>();

        if ( subject instanceof Resource && predicate instanceof URI )
        {
            try
            {
                RepositoryResult<Statement> stmtIter
                    = conn.getStatements(
//                        (Resource) subject, (URI) predicate, null, context, includeInferred );
                        (Resource) subject, (URI) predicate, null, Wurfel.useInference() );
                while ( stmtIter.hasNext() )
                    objects.add( stmtIter.next().getObject() );
                stmtIter.close();
            }

            catch ( Throwable t )
            {
                throw new WurfelException( t );
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
