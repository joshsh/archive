package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;

//import org.openrdf.OpenRDFException;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.querymodel.QueryLanguage;
import org.openrdf.queryresult.GraphQueryResult;
import org.openrdf.queryresult.TupleQueryResult;
import org.openrdf.queryresult.Solution;
import org.openrdf.repository.Connection;
import org.openrdf.repository.Repository;
import org.openrdf.rio.rdfxml.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.sail.Namespace;
import org.openrdf.util.iterator.CloseableIterator;

import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;

import java.util.Iterator;
import java.util.Set;
import java.util.HashSet;
import java.util.Collection;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Hashtable;
import java.util.LinkedHashSet;
import java.util.Observable;

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

        dereferencer = new HttpUriDereferencer();

        specialFunctions = new Hashtable<URI, Function>();

        ModelConnection mc = new ModelConnection( this, "for Model constructor" );

        try
        {
//            importModel( Wurfel.schemaUrl(), mc.createUri( "urn:wurfel" ), mc );

            ( new wurfel.extensions.test.TestExtension() ).load( mc );
            ( new wurfel.extensions.misc.MiscExtension() ).load( mc );
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
        RDFXMLWriter writer = new RDFXMLPrettyWriter( out );

        try
        {
            Connection con = repository.getConnection();
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
            }
        }

        Iterator<Value> resultIter = rdfMultiply( arg, func, mc.getConnection() ).iterator();
        Set<Value> result = new NodeSet();
        while ( resultIter.hasNext() )
        {
            Value v = resultIter.next();
            result.add( translateFromGraph( v ) );
        }

        return result;
    }

    ////////////////////////////////////////////////////////////////////////////

    // Note: this may be a very expensive operation (see Sesame API).
    public int countStatements()
        throws WurfelException
    {
        int size;

        try
        {
            Connection con = repository.getConnection();
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
            Connection conn = repository.getConnection();

            CloseableIterator<? extends Namespace> nsIter
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
        NodeSet contexts = new NodeSet();

        try
        {
            Connection conn = repository.getConnection();

            CloseableIterator<? extends Resource> contextIter
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
            Connection con = repository.getConnection();
            GraphQueryResult result = con.evaluateGraphQuery(
                QueryLanguage.SERQL, queryStr );
//                QueryLanguage.SERQL, "CONSTRUCT * FROM {x} p {y}");

// TODO: can I expect the Statements to remain valid after the connection is closed?
            for ( Statement st : result )
                statements.add( st );

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
            Connection con = repository.getConnection();
            TupleQueryResult result = con.evaluateTupleQuery(
                QueryLanguage.SERQL, queryStr );
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
                                   Connection conn )
        throws WurfelException
    {
        Set<Value> objects = new HashSet<Value>();

        if ( subject instanceof Resource && predicate instanceof URI )
        {
            try
            {
                CloseableIterator<? extends Statement> stmtIter
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
