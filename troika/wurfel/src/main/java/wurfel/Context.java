package wurfel;

import wurfel.model.Apply;
import wurfel.model.Model;
import wurfel.model.NodeSet;
import wurfel.model.Function;
import wurfel.model.Dereferencer;
import wurfel.model.HttpUriDereferencer;
import wurfel.model.EvaluationContext;

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
import org.openrdf.repository.RepositoryImpl;
import org.openrdf.rio.rdfxml.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.sail.Namespace;
import org.openrdf.sail.SailException;
import org.openrdf.sail.inferencer.MemoryStoreRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;
import org.openrdf.util.iterator.CloseableIterator;

import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;

import java.net.URL;

import java.util.Iterator;
import java.util.Set;
import java.util.Collection;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Hashtable;
import java.util.LinkedHashSet;

import org.apache.log4j.Logger;

public class Context
{
// FIXME
    private Resource singleContext;

    private Dereferencer dereferencer;
public Dereferencer getDereferencer()
{
    return dereferencer;
}

    private final static Logger s_logger = Logger.getLogger( Context.class );
//    private static final AdminListener s_adminListener
//        = new StdOutAdminListener();

    private final static boolean s_useInferencing = true;

// TODO: determine whether these URIs should really be created via the model's
//       graph's ValueFactory.
    private static URI
        s_xsdBooleanUri = null,
        s_xsdDoubleUri = null,
        s_xsdIntegerUri = null,
        s_xsdStringUri = null,
        s_rdfFirstUri = null,
        s_rdfRestUri = null,
        s_rdfNilUri = null;

    private static boolean s_initialized = false;

    private static void initialize()
        throws WurfelException
    {
        s_xsdBooleanUri = Wurfel.getXmlSchemaUri( "boolean" );
        s_xsdDoubleUri = Wurfel.getXmlSchemaUri( "double" );
        s_xsdIntegerUri = Wurfel.getXmlSchemaUri( "integer" );
        s_xsdStringUri = Wurfel.getXmlSchemaUri( "string" );
        s_rdfFirstUri = Wurfel.getRdfUri( "first" );
        s_rdfRestUri = Wurfel.getRdfUri( "rest" );
        s_rdfNilUri = Wurfel.getRdfUri( "nil" );

        s_initialized = true;
    }

    ////////////////////////////////////////////////////////////////////////////

    public Collection<Value> findProduct( Value arg, Value func, EvaluationContext evalContext )
        throws WurfelException
    {
//        Apply a = new Apply( func, arg );
//        return reduce( a );
        return multiply( arg, func, evalContext );
    }

    public Value findUniqueProduct( Value arg, Value func, EvaluationContext evalContext )
        throws WurfelException
    {
        Collection<Value> results = findProduct( arg, func, evalContext );

        if ( 1 != results.size() )
        {
            if ( 0 == results.size() )
                throw new WurfelException( "no values resolved for " + func.toString() + " of " + arg.toString() );

            else
                throw new WurfelException( func.toString() + " of " + arg.toString() + " resolved to more than one value" );
        }

        else
            return results.iterator().next();
    }

    public Resource castToResource( Value v )
        throws WurfelException
    {
        if ( v instanceof Resource )
            return (Resource) v;
        else
            throw new WurfelException( "value " + v.toString() + " is not a Resource" );
    }

    public URI castToUri( Value v )
        throws WurfelException
    {
        if ( v instanceof URI )
            return (URI) v;

        else
            throw new WurfelException( "value " + v.toString() + " is not a URI" );
    }

    public Literal castToLiteral( Value v )
        throws WurfelException
    {
        if ( v instanceof Literal )
            return (Literal) v;

        else
            throw new WurfelException( "value " + v.toString() + " is not a Literal" );
    }

    public boolean booleanValue( Literal l )
        throws WurfelException
    {
/*
        URI type = lit.getDatatype();
        if ( !type.equals( s_xsdBooleanUri ) )
            throw new WurfelException( "type mismatch: expected " + s_xsdBooleanUri.toString() + ", found " + type.toString() );
*/

        String label = l.getLabel();
// TODO: is capitalization relevant? Can 'true' also be represented as '1'?
        return label.equals( "true" );
    }

    public int intValue( Literal l )
        throws WurfelException
    {
/*
        URI type = l.getDatatype();
        if ( !type.equals( s_xsdIntegerUri ) )
            throw new WurfelException( "type mismatch: expected " + s_xsdIntegerUri.toString() + ", found " + type.toString() );
*/

        String label = l.getLabel();
        try
        {
            return ( new Integer( label ) ).intValue();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    public String stringValue( Literal l )
        throws WurfelException
    {
        return l.getLabel();
    }

    public List<Value> listValue( final Resource listHead )
        throws WurfelException
    {
        List<Value> list = new ArrayList<Value>();

        Resource cur = listHead;

// TODO: is this 'equals' safe?
        while ( !cur.equals( s_rdfNilUri ) )
        {
            Value val = findUniqueProduct( cur, s_rdfFirstUri );
            list.add( val );
            cur = castToResource( findUniqueProduct( cur, s_rdfRestUri ) );
        }

        return list;
    }

/*
    public Collection<Value> containerValue( final Resource head )
        throws WurfelException
    {


        try
        {
            Connection con = repository.getConnection();
            boolean includeInferred = false;
            CloseableIterator<? extends Statement> stmtIter
                = con.getStatements(
                    head, s_rdfTypeUri, s_swoogleQueryResponseUri, includeInferred );
            while ( stmtIter.hasNext() )
                results.add( stmtIter.next().getSubject() );
            stmtIter.close();
            conn.close();

            con.close();
        }

        catch ( OpenRDFException e )
        {
            throw new WurfelException( e );
        }
    }
*/

    ////////////////////////////////////////////////////////////////////////////

    public URI createUri( final String s )
    {
        return repository.getValueFactory().createURI( s );
    }

    public Literal createLiteral( final String s )
    {
        return model.getValueFactory().createLiteral( s, s_xsdStringUri );
    }

    public Literal createLiteral( final int i )
    {
        return model.getValueFactory().createLiteral( "" + i, s_xsdIntegerUri );
    }

    public Literal createLiteral( final double d )
    {
        return model.getValueFactory().createLiteral( "" + d, s_xsdDoubleUri );
    }

    ////////////////////////////////////////////////////////////////////////////

    String name;
    Repository repository;
    Collection<URL> importedDataURLs;

Hashtable<String, String> aliases;
Model model = null;

    private Hashtable<URI, Function> specialFunctions;

    public void addSpecialFunction( Function f )
    {
        specialFunctions.put( f.getUri(), f );
    }

    public Context( final String name )
        throws WurfelException
    {
        if ( !s_initialized )
            initialize();

        s_logger.debug( "Creating new Context '" + name + "'" );

aliases = new Hashtable<String, String>();
        this.name = name;
        importedDataURLs = new ArrayList<URL>();


        try
        {
            repository = new RepositoryImpl(
                new MemoryStoreRDFSInferencer(
                    new MemoryStore() ) );
//                    new MemoryStore( new java.io.File( "wurfel.tmp" ) ) ) );
            repository.initialize();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        singleContext = createUri( "urn:wurfel-context" );

        model = new Model( repository, singleContext );
        dereferencer = new HttpUriDereferencer( model, repository );

//System.out.println( "Wurfel.schemaUrl() = " + Wurfel.schemaUrl() );
        importModel( Wurfel.schemaUrl(), createUri( "urn:wurfel" ) );
//        importModel( Wurfel.testUrl(), createUri( "urn:wurfel-test" ) );

        specialFunctions = new Hashtable<URI, Function>();

        ( new wurfel.extensions.test.TestExtension( this ) ).load();
        ( new wurfel.extensions.misc.MiscExtension( this ) ).load();
    }

public Repository getRepository()
{
    return repository;
}
//private boolean namespacesDefined = false;

    public void importModel( final URL url, final URI baseURI )
        throws WurfelException
    {
        model.dereferenceGraph( url, baseURI );
        importedDataURLs.add( url );
    }

    public void addStatement( Value subj, Value pred, Value obj )
        throws WurfelException
    {
        Resource subjResource = castToResource( subj );
        URI predUri = castToUri( pred );

        try
        {
            Connection con = repository.getConnection();
            con.add( subjResource, predUri, obj, singleContext );
            con.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

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

    private Value translateToGraph( Value v )
    {
        if ( v instanceof Function )
            return ( (Function) v ).getUri();
        else
            return v;
    }

    private Set<Value> multiply( Value arg, Value func, EvaluationContext evalContext )
        throws WurfelException
    {
        if ( arg instanceof URI )
        {
            try
            {
                dereferencer.dereferenceSubjectUri( (URI) arg, evalContext );
            }

            catch ( WurfelException e )
            {
                // (soft fail)
            }
        }

        return model.multiply( arg, func /*, evalContext*/ );
    }

// FIXME: 'apply' is now a bit of a misnomer
    public Set<Value> apply( Value func, Value arg, EvaluationContext evalContext )
        throws WurfelException
    {
        arg = translateToGraph( arg );

        Iterator<Value> resultIter = multiply( arg, func, evalContext ).iterator();
        Set<Value> result = new NodeSet();
        while ( resultIter.hasNext() )
        {
            Value v = resultIter.next();
            result.add( translateFromGraph( v ) );
        }

        return result;
    }

    //FIXME: temporary method
    public Model getModel()
    {
        return model;
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

    public void showContextIds()
        throws WurfelException
    {
        try
        {
            Connection conn = repository.getConnection();

            CloseableIterator<? extends Resource> contextIter
                 = conn.getContextIDs();
            while ( contextIter.hasNext() )
            {
                Resource context = contextIter.next();
                System.out.println( context.toString() );
            }
            contextIter.close();

            conn.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
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

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
