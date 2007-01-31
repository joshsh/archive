package wurfel;


import wurfel.Wurfel;
import wurfel.model.Apply;
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
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.rdfxml.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.sail.Namespace;
import org.openrdf.sail.SailException;
import org.openrdf.sail.inferencer.MemoryStoreRDFSInferencer;
import org.openrdf.sail.memory.MemoryStore;
import org.openrdf.util.iterator.CloseableIterator;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;

import java.net.URL;
import java.net.URLConnection;

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

public class Context extends Observable
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

    String name;
    Repository repository;

Hashtable<String, String> aliases;

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

        dereferencer = new HttpUriDereferencer( this );

        specialFunctions = new Hashtable<URI, Function>();

        EvaluationContext evalContext = new EvaluationContext( this, "for Context constructor" );

        try
        {
            importModel( Wurfel.schemaUrl(), evalContext.createUri( "urn:wurfel" ), evalContext );

            ( new wurfel.extensions.test.TestExtension() ).load( evalContext );
            ( new wurfel.extensions.misc.MiscExtension() ).load( evalContext );
        }

        catch ( WurfelException e )
        {
            evalContext.close();
            throw e;
        }

        evalContext.close();
    }

public Repository getRepository()
{
    return repository;
}
//private boolean namespacesDefined = false;

    public void importModel( final URL url, final URI baseURI, EvaluationContext evalContext )
        throws WurfelException
    {
        dereferenceGraph( url, baseURI, evalContext.getConnection() );
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

// FIXME: will return null if the Function is an Apply
    private Value translateToGraph( Value v )
    {
        if ( v instanceof Function )
            return ( (Function) v ).getUri();
        else
            return v;
    }

// TODO: this operation is a little counterintuitive, in that it does not apply primitive functions
    public Set<Value> multiply( Value arg, Value func, EvaluationContext evalContext )
        throws WurfelException
    {
        arg = translateToGraph( arg );

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

        Iterator<Value> resultIter = rdfMultiply( arg, func, evalContext.getConnection() ).iterator();
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

    private void close( InputStream is )
        throws WurfelException
    {
        try
        {
            is.close();
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }
    }

    // Note: examines the content type first, then the URL extension.  If all
    //       else fails, try reading as RDF/XML and hope for the best.
    private RDFFormat guessRdfFormat( final URLConnection urlConn )
    {
/*
System.out.println( RDFFormat.N3.getName() + ": " + RDFFormat.N3.getMIMEType() );
System.out.println( RDFFormat.NTRIPLES.getName() + ": " + RDFFormat.NTRIPLES.getMIMEType() );
System.out.println( RDFFormat.RDFXML.getName() + ": " + RDFFormat.RDFXML.getMIMEType() );
System.out.println( RDFFormat.TRIX.getName() + ": " + RDFFormat.TRIX.getMIMEType() );
System.out.println( RDFFormat.TURTLE.getName() + ": " + RDFFormat.TURTLE.getMIMEType() );
*/
        String contentType = urlConn.getContentType();
System.out.println( "######## contentType = " + contentType );

        String file = urlConn.getURL().getFile();
        String ext;
        if ( null == file )
            ext = null;
        else
        {
            int lastDot = file.lastIndexOf( '.' );
            ext = ( lastDot > 0 && lastDot < file.length() - 1 )
                ? file.substring( lastDot + 1 )
                : null;
        }
System.out.println( "######## ext = " + ext );

        // Primary content type rules.
        if ( null != contentType )
        {
            // See: http://www.w3.org/TR/rdf-syntax-grammar/
            if ( contentType.contains( "application/rdf+xml" ) )
                return RDFFormat.RDFXML;

            // See: http://www.w3.org/DesignIssues/Notation3.html
            else if ( contentType.contains( "text/rdf+n3" ) )
                return RDFFormat.N3;

// See: RDFFormat.TRIX.getMIMEType()
            else if ( contentType.contains( "application/trix" ) )
                return RDFFormat.TRIX;

            // See: http://www.dajobe.org/2004/01/turtle/
            else if ( contentType.contains( "application/x-turtle" ) )
                return RDFFormat.N3;
        }

        // Primary file extension rules.
        if ( null != ext )
        {
// TODO: I don't know if this is actually an N3 file extension
            if ( ext.equals( "n3" ) )
                return RDFFormat.N3;

            else if ( ext.equals( "nt" ) )
                return RDFFormat.NTRIPLES;

            else if ( ext.equals( "rdf" ) )
                return RDFFormat.RDFXML;

// TODO: I don't know if this is actually a TriX file extension
            else if ( ext.equals( "trix" ) )
                return RDFFormat.TRIX;

            else if ( ext.equals( "ttl" ) )
                return RDFFormat.TURTLE;

// TODO: I'm not sure just how hackish this is.
// precedent:
//     http://www.aaronsw.com/about.xrdf
//     http://www.w3.org/People/karl/karl-foaf.xrdf
            else if ( ext.equals( "xrdf" ) )
                return RDFFormat.RDFXML;
        }

        // Secondary content type rules.
        if ( null != contentType )
        {
            if ( contentType.contains( "application/xml" ) )
                return RDFFormat.RDFXML;

            // precedent: http://www.mindswap.org/2004/owl/mindswappers
            else if ( contentType.contains( "text/xml" ) )
                return RDFFormat.RDFXML;

            // See: http://www.w3.org/TR/rdf-testcases/#ntriples)
            // This is only a secondary rule because the text/plain MIME type
            // is so broad, and the N-Triples format so uncommon.
//            else if ( contentType.contains( "text/plain" ) )
//                return RDFFormat.NTRIPLES;
        }

        // Secondary file extension rules.
        if ( null != ext )
        {
            // precedent:
            //     http://hometown.aol.com/chbussler/foaf/chbussler.foaf
            if ( ext.equals( "foaf" ) )
                return RDFFormat.RDFXML;
        }

        // Last-ditch rule.
        return RDFFormat.RDFXML;
    }

    public void dereferenceGraph( final URL url, final URI baseURI, Connection conn )
        throws WurfelException
    {
        s_logger.debug( "Importing model " + url.toString() +
            ( ( null == baseURI ) ? "" : " as " + baseURI.toString() ) );

        boolean verifyData = true;

        URLConnection urlConn;
        InputStream response;

System.out.println( "######## dereferencing graph in model: " + url );

        try
        {
            urlConn = url.openConnection();
            urlConn.connect();
            response = urlConn.getInputStream();
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }

        RDFFormat format = guessRdfFormat( urlConn );
        if ( null == format )
        {
            close( response );
            return;
        }
System.out.println( "####### Guessed format is " + format.getName() );

        try
        {
            if ( null == baseURI )
                conn.add( response, null, format );
            else
                conn.add( response, baseURI.toString(), format, baseURI );
        }

        catch ( Throwable t  )
        {
            close( response );
            throw new WurfelException( t );
        }

        close( response );

System.out.println( "####### graph successfully imported" );

        setChanged();
        notifyObservers();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
