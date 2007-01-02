package wurfel;

import wurfel.model.Model;
import wurfel.model.ModelMock;
import wurfel.model.Apply;
import wurfel.model.NodeSet;
import wurfel.model.Function;

//import org.openrdf.OpenRDFException;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.querylanguage.MalformedQueryException;
import org.openrdf.querylanguage.UnsupportedQueryLanguageException;
import org.openrdf.querymodel.QueryLanguage;
import org.openrdf.queryresult.GraphQueryResult;
import org.openrdf.queryresult.TupleQueryResult;
import org.openrdf.queryresult.Solution;
import org.openrdf.repository.Connection;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryImpl;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandlerException;
import org.openrdf.rio.RDFParseException;
import org.openrdf.rio.UnsupportedRDFormatException;
import org.openrdf.rio.rdfxml.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.sail.Namespace;
import org.openrdf.sail.SailException;
import org.openrdf.sail.SailInitializationException;
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

    private final static Logger s_logger = Logger.getLogger( Context.class );
//    private static final AdminListener s_adminListener
//        = new StdOutAdminListener();


    private final static boolean s_useInferencing = true;

// TODO: determine whether these URIs should really be created via the model's
//       graph's ValueFactory.
    private static final URI
        s_xsdBooleanUri = Wurfel.getXmlSchemaUri( "boolean" ),
        s_xsdDoubleUri = Wurfel.getXmlSchemaUri( "double" ),
        s_xsdIntegerUri = Wurfel.getXmlSchemaUri( "integer" ),
        s_xsdStringUri = Wurfel.getXmlSchemaUri( "string" ),
        s_rdfFirstUri = Wurfel.getRdfUri( "first" ),
        s_rdfRestUri = Wurfel.getRdfUri( "rest" ),
        s_rdfNilUri = Wurfel.getRdfUri( "nil" );

    ////////////////////////////////////////////////////////////////////////////

    public Collection<Value> findProduct( Value arg, Value func )
        throws WurfelException
    {
//        Apply a = new Apply( func, arg );
//        return reduce( a );
        return model.multiply( arg, func );
    }

    public Value findUniqueProduct( Value arg, Value func )
        throws WurfelException
    {
        Collection<Value> results = findProduct( arg, func );

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

        catch ( SailInitializationException e )
        {
            throw new WurfelException( e );
        }

        singleContext = createUri( "urn:wurfel-context" );

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
        s_logger.debug( "Importing model " + url.toString() +
            ( ( null == baseURI ) ? "" : " as " + baseURI.toString() ) );

        boolean verifyData = true;

        try
        {
            Connection con = repository.getConnection();
//            con.add( url, baseURI, RDFFormat.RDFXML, singleContext );
            if ( null == baseURI )
                con.add( url, null, RDFFormat.RDFXML );
            else
                con.add( url, baseURI.toString(), RDFFormat.RDFXML, baseURI );
/*
if ( !namespacesDefined )
{
    con.setNamespace( "wurfel", "urn:net.dnsdojo.troika.wurfel#" );
    con.setNamespace( "wurfel", "urn:net.dnsdojo.troika.wurfel#" );
    con.setNamespace( "wurfel", "urn:net.dnsdojo.troika.wurfel#" );
    con.setNamespace( "wurfel-test", "urn:net.dnsdojo.troika.wurfel-test#" );
    namespacesDefined = true;
}
*/
            con.close();
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        catch ( UnsupportedRDFormatException e )
        {
            throw new WurfelException( e );
        }

        catch ( RDFParseException e )
        {
            throw new WurfelException( e );
        }

        importedDataURLs.add( url );
        updateModel();
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        catch ( RDFHandlerException e )
        {
            throw new WurfelException( e );
        }
//        updateModel();
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

    private Value translateFromGraph( Value v )
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

    // TODO: this should throw an exception if the identifier does not resolve
    //       to a unique value.
    public Value resolveIdentifier( String s )
        throws WurfelException
    {
        Value v = model.resolve( s );
        return ( v == null ) ? null : translateFromGraph( v );
    }

// FIXME: 'apply' is now a bit of a misnomer
    public Set<Value> apply( Value func, Value arg )
        throws WurfelException
    {
        arg = translateToGraph( arg );

// TODO: combinators and primitives as func will have their own, idiosyncratic ways of yielding a product
        Iterator<Value> resultIter = model.multiply( arg, func ).iterator();
        Set<Value> result = new NodeSet();
        while ( resultIter.hasNext() )
        {
            Value v = resultIter.next();
            result.add( translateFromGraph( v ) );
        }

        return result;
    }

    private void updateModel()
        throws WurfelException
    {
        model = new ModelMock( repository, singleContext );
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        return size;
    }

    public void define( String name, String uri )
    {
        aliases.put( name, uri );
    }

    private String uriAbbr( final URI uri )
        throws WurfelException
    {
        String prefix = model.nsPrefixOf( uri );
        if ( null == prefix )
            return uri.toString();
        else
            return prefix + ":" + uri.getLocalName();
    }

    public void show( Resource subject )
        throws WurfelException
    {
        if ( subject instanceof URI )
            System.out.println( uriAbbr( (URI) subject ) );
        else
            System.out.println( subject.toString() );

        Set<URI> predicates = model.getPredicates( subject );
        Iterator<URI> predIter = predicates.iterator();
        while ( predIter.hasNext() )
        {
            URI predicate = predIter.next();

            System.out.println( "    " + uriAbbr( predicate ) );
//            System.out.println( "    " + predicate.getLocalName() );

            Set<Value> objects = model.multiply( subject, predicate );
            Iterator<Value> objIter = objects.iterator();
            while ( objIter.hasNext() )
            {
                Value obj = objIter.next();
                if ( obj instanceof URI )
                    System.out.println( "        " + uriAbbr( (URI) obj ) );
//                    System.out.println( "        " + ( (URI) obj ).getLocalName() );
                else
                    System.out.println( "        " + obj.toString() );
            }
        }
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }
    }

    public void printStatements()
        throws WurfelException
    {
        updateModel();

        Set<Resource> subjects = model.getSubjects();
        Iterator<Resource> subjIter = subjects.iterator();
        while ( subjIter.hasNext() )
        {
            Resource subject = subjIter.next();
            show( subject );
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        catch ( MalformedQueryException e )
        {
            throw new WurfelException( e );
        }

        catch ( UnsupportedQueryLanguageException e )
        {
            throw new WurfelException( e );
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

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        catch ( MalformedQueryException e )
        {
            throw new WurfelException( e );
        }

        catch ( UnsupportedQueryLanguageException e )
        {
            throw new WurfelException( e );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
