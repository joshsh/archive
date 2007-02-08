package wurfel.model;

import java.io.InputStream;
import java.io.IOException;

import java.net.URL;
import java.net.URLConnection;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.Random;

import org.apache.log4j.Logger;

import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.repository.Connection;
import org.openrdf.rio.RDFFormat;
import org.openrdf.util.iterator.CloseableIterator;

import wurfel.Context;
import wurfel.Wurfel;
import wurfel.WurfelException;

public class EvaluationContext
{
    private final static Logger s_logger = Logger.getLogger( Context.class );

    private Context context;
    private Connection connection;
    private String name = null;

    private static URI
        s_xsdBooleanUri = null,
        s_xsdDoubleUri = null,
        s_xsdIntegerUri = null,
        s_xsdStringUri = null,
        s_rdfFirstUri = null,
        s_rdfRestUri = null,
        s_rdfNilUri = null;

    private static boolean s_initialized = false;

    ////////////////////////////////////////////////////////////////////////////

    private void constructPrivate( Context context )
        throws WurfelException
    {
        this.context = context;

        if ( !s_initialized )
        {
            s_xsdBooleanUri = createXmlSchemaUri( "boolean" );
            s_xsdDoubleUri = createXmlSchemaUri( "double" );
            s_xsdIntegerUri = createXmlSchemaUri( "integer" );
            s_xsdStringUri = createXmlSchemaUri( "string" );
            s_rdfFirstUri = createRdfUri( "first" );
            s_rdfRestUri = createRdfUri( "rest" );
            s_rdfNilUri = createRdfUri( "nil" );

            s_initialized = true;
        }

        try
        {
            connection = context.getRepository().getConnection();
//System.out.println( "Opened "
//    + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//    + " (" + openConnections + " total)." );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        add( this );
    }

    public EvaluationContext( Context context )
        throws WurfelException
    {
        constructPrivate( context );
    }

    public EvaluationContext( Context context, final String name )
        throws WurfelException
    {
        this.name = name;
        constructPrivate( context );
    }

    public Context getContext()
    {
        return context;
    }

    public Connection getConnection()
    {
        return connection;
    }

    public void close()
        throws WurfelException
    {
        try
        {
            connection.close();
//System.out.println( "Closed "
//    + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//    + " (" + openConnections + " total)." );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        remove( this );
    }

    ////////////////////////////////////////////////////////////////////////////

    private static Set<EvaluationContext> openConnections
        = new LinkedHashSet<EvaluationContext>();

    private static void add( EvaluationContext evalContext )
    {
        synchronized( openConnections )
        {
            openConnections.add( evalContext );
        }
    }

    private static void remove( EvaluationContext evalContext )
    {
        synchronized( openConnections )
        {
            openConnections.remove( evalContext );
        }
    }

    public static List<String> listOpenConnections()
    {
        synchronized( openConnections )
        {
            List<String> names = new ArrayList<String>( openConnections.size() );

            Iterator<EvaluationContext> i = openConnections.iterator();
            while ( i.hasNext() )
                names.add( i.next().name );

            return names;
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public Collection<Value> findProduct( Value arg, Value func )
        throws WurfelException
    {
//        Apply a = new Apply( func, arg );
//        return reduce( a );
        return context.multiply( arg, func, this );
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


    public Collection<Value> bagValue( final Resource head )
        throws WurfelException
    {
        Collection<Value> results = new NodeSet();

        try
        {
            boolean useInference = false;

            CloseableIterator<? extends Statement> stmtIter
                = connection.getStatements(
                    head, null, null, useInference );

            while ( stmtIter.hasNext() )
            {
                Statement st = stmtIter.next();
                if ( '_' == st.getPredicate().getLocalName().charAt( 0 ) )
                    results.add( st.getObject() );
            }

            stmtIter.close();
        }

        // Warning: the CloseableIterator may be left open.
        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return results;
    }


    ////////////////////////////////////////////////////////////////////////////

/*
    public Set<Resource> getSubjects()
        throws WurfelException
    {
        Set<Resource> subjects = new HashSet<Resource>();

        try
        {
            CloseableIterator<? extends Statement> stmtIter
                = connection.getStatements(
//                    null, null, null, context, includeInferred );
                    null, null, null, Wurfel.useInference() );
            while ( stmtIter.hasNext() )
                subjects.add( stmtIter.next().getSubject() );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return subjects;
    }
*/

    public Set<URI> getPredicates( Resource subject )
        throws WurfelException
    {
        Set<URI> predicates = new HashSet<URI>();

        try
        {
            CloseableIterator<? extends Statement> stmtIter
                = connection.getStatements(
//                    subject, null, null, context, includeInferred );
                    subject, null, null, Wurfel.useInference() );
            while ( stmtIter.hasNext() )
                predicates.add( stmtIter.next().getPredicate() );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }

        return predicates;
    }

    public void addStatement( Value subj, Value pred, Value obj )
        throws WurfelException
    {
        Resource subjResource = castToResource( subj );
        URI predUri = castToUri( pred );

        try
        {
//            connection.add( subjResource, predUri, obj, singleContext );
            connection.add( subjResource, predUri, obj );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public URI createUri( final String s )
    {
        return context.getRepository().getValueFactory().createURI( s );
    }

    public Literal createLiteral( final String s )
    {
        return context.getRepository().getValueFactory().createLiteral( s, s_xsdStringUri );
    }

    public Literal createLiteral( final boolean b )
    {
        return context.getRepository().getValueFactory().createLiteral( "" + b, s_xsdBooleanUri );
    }

    public Literal createLiteral( final int i )
    {
        return context.getRepository().getValueFactory().createLiteral( "" + i, s_xsdIntegerUri );
    }

    public Literal createLiteral( final double d )
    {
        return context.getRepository().getValueFactory().createLiteral( "" + d, s_xsdDoubleUri );
    }

    private static Random s_rn = new Random();

    private static int randomInt( int lo, int hi )
    {
        int n = hi - lo + 1;
        int i = s_rn.nextInt() % n;
        if (i < 0)
            i = -i;
        return lo + i;
    }

    public URI createRandomUri()
        throws WurfelException
    {
        return createUri( "urn:random:" + randomInt( 0, Integer.MAX_VALUE ) );
    }

    public URI createRdfUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://www.w3.org/1999/02/22-rdf-syntax-ns#" + localName );
    }

    public URI createRdfSchemaUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://www.w3.org/2000/01/rdf-schema#" + localName );
    }

    public URI createWurfelUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://fortytwo.net/2007/02/06/wurfel#" + localName );
    }

    public URI createWurfelTestUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://fortytwo.net/2007/02/06/wurfel-test#" + localName );
    }

    public URI createWurfelMiscUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://fortytwo.net/2007/02/06/wurfel-misc#" + localName );
    }

    public URI createXmlSchemaUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://www.w3.org/2001/XMLSchema#" + localName );
    }

    public URI createSwoogleUri( final String localName )
        throws WurfelException
    {
        return createUri( "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#" + localName );
    }

    ////////////////////////////////////////////////////////////////////////////

    // Note: examines the content type first, then the URL extension.  If all
    //       else fails, default to RDF/XML and hope for the best.
    private static RDFFormat guessRdfFormat( final URLConnection urlConn )
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

            else if ( ext.equals( "rdf" )
              || ext.equals( "rdfs" )
              || ext.equals( "owl" ) )
                return RDFFormat.RDFXML;

// TODO: I don't know if this is actually a TriX file extension
            else if ( ext.equals( "trix" ) )
                return RDFFormat.TRIX;

            else if ( ext.equals( "ttl" )
              || ext.equals( "turtle" ) )
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

        // Blacklisting rules.  There are some common content types which are
        // not worth trying.
        if ( null != contentType )
        {
            if ( contentType.contains( "text/html" ) )
                return null;
        }

        // Last-ditch rule.
        return RDFFormat.RDFXML;
    }

    private static void showUrlConnection( URLConnection urlConn )
    {
        Map<String,List<String>> requestProperties
            = urlConn.getRequestProperties();
        Set<String> keys = requestProperties.keySet();

        StringBuilder sb = new StringBuilder();
        sb.append( "Request properties:\n" );

        Iterator<String> keyIter = keys.iterator();
        while ( keyIter.hasNext() )
        {
            String key = keyIter.next();
            sb.append( "\t" + key + ": " );
            Iterator<String> valueIter = requestProperties.get( key ).iterator();

            boolean first = true;
            while ( valueIter.hasNext() )
            {
                String value = valueIter.next();
                if ( first )
                    first = false;
                else
                    sb.append( ", " );
                sb.append( value );
            }

            sb.append( "\n" );
        }

        System.out.println( sb.toString() );
    }

    private static void prepareUrlConnectionForRdfRequest( URLConnection urlConn )
    {
        urlConn.addRequestProperty( "User-Agent",
            Wurfel.getWurfelName() + "/" + Wurfel.getWurfelVersion() );

        // Add values in the reverse order in which they will appear in the header.
        urlConn.addRequestProperty( "Accept", "text/xml;q=0.2" );
        urlConn.addRequestProperty( "Accept", "application/xml;q=0.5" );
        urlConn.addRequestProperty( "Accept", "text/plain" );
        urlConn.addRequestProperty( "Accept", "application/x-turtle" );
        urlConn.addRequestProperty( "Accept", "application/trix" );
        urlConn.addRequestProperty( "Accept", "text/rdf+n3" );
        urlConn.addRequestProperty( "Accept", "application/rdf+xml" );

// To consider at some point: caching, authorization
    }

    public void addGraph( final URL url, final URI baseURI )
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
            prepareUrlConnectionForRdfRequest( urlConn );
showUrlConnection( urlConn );

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
                connection.add( response, null, format );
            else
                connection.add( response, baseURI.toString(), format, baseURI );
        }

        catch ( Throwable t  )
        {
            close( response );
            throw new WurfelException( t );
        }

        close( response );

System.out.println( "####### graph successfully imported" );

        context.touch();
    }

    public void addGraph( final URL url )
        throws WurfelException
    {
        String uriStr;

        try
        {
            uriStr = url.toURI().toString();
        }

        catch ( java.net.URISyntaxException e )
        {
            throw new WurfelException( e );
        }

        URI baseURI = createUri( uriStr );
        addGraph( url, baseURI );
    }

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
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
