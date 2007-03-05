package net.fortytwo.ripple.model;

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

import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParseException;
import org.openrdf.repository.RepositoryResult;

import net.fortytwo.ripple.ThreadWrapper;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

public class ModelConnection
{
    private final static Logger s_logger
        = Logger.getLogger( ModelConnection.class );

    private Model model;
    private RepositoryConnection repoConnection;
    private String name = null;

    ////////////////////////////////////////////////////////////////////////////

    private void constructPrivate( Model model )
        throws RippleException
    {
        this.model = model;

        try
        {
            repoConnection = model.getRepository().getConnection();
//System.out.println( "Opened "
//    + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//    + " (" + openConnections + " total)." );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        add( this );

        applyOp = createUri( "http://fortytwo.net/2007/02/ripple#apply" );
    }

    public ModelConnection( Model model )
        throws RippleException
    {
        constructPrivate( model );
    }

    public ModelConnection( Model model, final String name )
        throws RippleException
    {
        this.name = name;
        constructPrivate( model );
    }

    private URI applyOp;

    public URI getApplyOp()
    {
        return applyOp;
    }

    public Model getModel()
    {
        return model;
    }

    public RepositoryConnection getRepositoryConnection()
    {
        return repoConnection;
    }

    public void close()
        throws RippleException
    {
        try
        {
            repoConnection.close();
//System.out.println( "Closed "
//    + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//    + " (" + openConnections + " total)." );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        remove( this );
    }

    ////////////////////////////////////////////////////////////////////////////

    private static Set<ModelConnection> openConnections
        = new LinkedHashSet<ModelConnection>();

    private static void add( ModelConnection mc )
    {
        synchronized( openConnections )
        {
            openConnections.add( mc );
        }
    }

    private static void remove( ModelConnection mc )
    {
        synchronized( openConnections )
        {
            openConnections.remove( mc );
        }
    }

    public static List<String> listOpenConnections()
    {
        synchronized( openConnections )
        {
            List<String> names = new ArrayList<String>( openConnections.size() );

            Iterator<ModelConnection> i = openConnections.iterator();
            while ( i.hasNext() )
                names.add( i.next().name );

            return names;
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public Collection<Value> findProduct( Value arg, Value func )
        throws RippleException
    {
//        Apply a = new Apply( func, arg );
//        return reduce( a );
        return model.multiply( arg, func, this );
    }

    public Value findUniqueProduct( Value arg, Value func )
        throws RippleException
    {
        Collection<Value> results = findProduct( arg, func );

        if ( 1 != results.size() )
        {
            if ( 0 == results.size() )
                throw new RippleException( "no values resolved for " + func.toString() + " of " + arg.toString() );

            else
                throw new RippleException( func.toString() + " of " + arg.toString() + " resolved to more than one value" );
        }

        else
            return results.iterator().next();
    }

    public Resource castToResource( Value v )
        throws RippleException
    {
        if ( v instanceof Resource )
            return (Resource) v;
        else
            throw new RippleException( "value " + v.toString() + " is not a Resource" );
    }

    public URI castToUri( Value v )
        throws RippleException
    {
        if ( v instanceof URI )
            return (URI) v;

        else
            throw new RippleException( "value " + v.toString() + " is not a URI" );
    }

    public Literal castToLiteral( Value v )
        throws RippleException
    {
        if ( v instanceof Literal )
            return (Literal) v;

        else
            throw new RippleException( "value " + v.toString() + " is not a Literal" );
    }

    public boolean booleanValue( Literal l )
        throws RippleException
    {
/*
        URI type = lit.getDatatype();
        if ( !type.equals( XMLSchema.BOOLEAN ) )
            throw new RippleException( "type mismatch: expected " + XMLSchema.BOOLEAN.toString() + ", found " + type.toString() );
*/

        String label = l.getLabel();
// TODO: is capitalization relevant? Can 'true' also be represented as '1'?
        return label.equals( "true" );
    }

    public int intValue( Literal l )
        throws RippleException
    {
/*
        URI type = l.getDatatype();
        if ( !type.equals( XMLSchema.INTEGER ) )
            throw new RippleException( "type mismatch: expected " + XMLSchema.INTEGER.toString() + ", found " + type.toString() );
*/

        String label = l.getLabel();
        try
        {
            return ( new Integer( label ) ).intValue();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public String stringValue( Literal l )
        throws RippleException
    {
        return l.getLabel();
    }

    public List<Value> listValue( final Resource listHead )
        throws RippleException
    {
        List<Value> list = new ArrayList<Value>();

        Resource cur = listHead;

// TODO: is this 'equals' safe?
        while ( !cur.equals( RDF.NIL ) )
        {
            Value val = findUniqueProduct( cur, RDF.FIRST );
            list.add( val );
            cur = castToResource( findUniqueProduct( cur, RDF.REST ) );
        }

        return list;
    }


    public Collection<Value> bagValue( final Resource head )
        throws RippleException
    {
        Collection<Value> results = new Container();

        try
        {
            boolean useInference = false;

            RepositoryResult<Statement> stmtIter
                = repoConnection.getStatements(
                    head, null, null, useInference );

            while ( stmtIter.hasNext() )
            {
                Statement st = stmtIter.next();
                if ( '_' == st.getPredicate().getLocalName().charAt( 0 ) )
                    results.add( st.getObject() );
            }

            stmtIter.close();
        }

        // Warning: the RepositoryResult may be left open.
        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return results;
    }


    ////////////////////////////////////////////////////////////////////////////

/*
    public Set<Resource> getSubjects()
        throws RippleException
    {
        Set<Resource> subjects = new HashSet<Resource>();

        try
        {
            RepositoryResult<Statement> stmtIter
                = repoConnection.getStatements(
//                    null, null, null, model, includeInferred );
                    null, null, null, Ripple.useInference() );
            while ( stmtIter.hasNext() )
                subjects.add( stmtIter.next().getSubject() );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return subjects;
    }
*/

    public Set<URI> getPredicates( Resource subject )
        throws RippleException
    {
        Set<URI> predicates = new HashSet<URI>();

        try
        {
            RepositoryResult<Statement> stmtIter
                = repoConnection.getStatements(
//                    subject, null, null, model, includeInferred );
                    subject, null, null, Ripple.useInference() );
            while ( stmtIter.hasNext() )
                predicates.add( stmtIter.next().getPredicate() );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return predicates;
    }

    public void addStatement( Value subj, Value pred, Value obj )
        throws RippleException
    {
        Resource subjResource = castToResource( subj );
        URI predUri = castToUri( pred );

        try
        {
//            repoConnection.add( subjResource, predUri, obj, singleContext );
            repoConnection.add( subjResource, predUri, obj );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public void addStatement( Resource subj, URI pred, Value obj, Resource context )
        throws RippleException
    {
        try
        {
            repoConnection.add( subj, pred, obj, context );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public URI createUri( final String s )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createURI( s );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public URI createUri( final String ns, final String s )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createURI( ns + s );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public URI createUri( final URI ns, final String s )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createURI( ns.toString() + s );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Literal createLiteral( final String s )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createLiteral( s, XMLSchema.STRING );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Literal createLiteral( final String s, final String language )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createLiteral( s, language );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Literal createLiteral( final String s, final URI dataType )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createLiteral( s, dataType );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Literal createLiteral( final boolean b )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createLiteral( "" + b, XMLSchema.BOOLEAN );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Literal createLiteral( final int i )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createLiteral( "" + i, XMLSchema.INTEGER );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Literal createLiteral( final double d )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createLiteral( "" + d, XMLSchema.DOUBLE );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public BNode createBNode()
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createBNode();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public BNode createBNode( final String id )
        throws RippleException
    {
        try
        {
            return model.getRepository().getValueFactory().createBNode( id );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
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
        throws RippleException
    {
        return createUri( "urn:random:" + randomInt( 0, Integer.MAX_VALUE ) );
    }

    public URI createRdfUri( final String localName )
        throws RippleException
    {
        return createUri( "http://www.w3.org/1999/02/22-rdf-syntax-ns#" + localName );
    }

    public URI createRdfSchemaUri( final String localName )
        throws RippleException
    {
        return createUri( "http://www.w3.org/2000/01/rdf-schema#" + localName );
    }

    public URI createRippleUri( final String localName )
        throws RippleException
    {
        return createUri( "http://fortytwo.net/2007/02/06/wurfel#" + localName );
    }

    public URI createRippleTestUri( final String localName )
        throws RippleException
    {
        return createUri( "http://fortytwo.net/2007/02/06/wurfel-test#" + localName );
    }

    public URI createRippleMiscUri( final String localName )
        throws RippleException
    {
        return createUri( "http://fortytwo.net/2007/02/06/wurfel-misc#" + localName );
    }

    public URI createXmlSchemaUri( final String localName )
        throws RippleException
    {
        return createUri( "http://www.w3.org/2001/XMLSchema#" + localName );
    }

    public URI createSwoogleUri( final String localName )
        throws RippleException
    {
        return createUri( "http://daml.umbc.edu/ontologies/webofbelief/1.4/swoogle.owl#" + localName );
    }

    ////////////////////////////////////////////////////////////////////////////

    public void add( Resource subject, URI predicate, Value object )
        throws RippleException
    {
// FIXME
Resource defaultContext = null;
        try
        {
            repoConnection.add( subject, predicate, object, defaultContext );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Value toRdf( final Value src )
        throws RippleException
    {
        return ( src instanceof RippleValue )
            ? ( (RippleValue) src ).toRdf( this )
            : src;
    }

    ////////////////////////////////////////////////////////////////////////////

    public void setNamespace( final String prefix, final URI ns )
        throws RippleException
    {
        try
        {
            repoConnection.removeNamespace( prefix );
            repoConnection.setNamespace( prefix, ns.toString() );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
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
s_logger.info( "######## contentType = " + contentType );

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
s_logger.info( "######## ext = " + ext );

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
        urlConn.setRequestProperty( "User-Agent",
            Ripple.getRippleName() + "/" + Ripple.getRippleVersion() );

        /* Comment by arjohn in http://www.openrdf.org/forum/mvnforum/viewthread?thread=805#3234
               Note that Sesame/Rio doesn't have a real N3 parser, but it does have a Turtle parser, which supports a much larger subset of N3. At first sight, I would say that the Turtle parser should be able to parse the data fragment that you posted. */
        boolean n3DeserializationSupported = false;

        StringBuilder sb = new StringBuilder();

        sb.append( "application/rdf+xml" );

        if ( n3DeserializationSupported )
            sb.append( ", text/rdf+n3" );

        sb.append( ", application/trix" );
        sb.append( ", application/x-turtle" );
        sb.append( ", text/plain" );
        sb.append( ", application/xml;q=0.5" );
        sb.append( ", text/xml;q=0.2" );

        urlConn.setRequestProperty( "Accept", sb.toString() );


// To consider at some point: caching, authorization
    }

    private URLConnection openUrlConnection( URL url )
        throws RippleException
    {
        URLConnection urlConn;

        try
        {
            urlConn = url.openConnection();
            prepareUrlConnectionForRdfRequest( urlConn );
showUrlConnection( urlConn );
        }

        catch ( IOException e )
        {
            throw new RippleException( e );
        }

        try
        {
            urlConn.connect();
        }

        catch ( IOException e )
        {
            throw new RippleException( e );
        }

        return urlConn;
    }


    public void addGraph( final URL url, final URI baseURI )
        throws RippleException
    {
        // Wrap the entire operation in the timeout wrapper, as there are various
        // pieces which are capable of hanging:
        //     urlConn.connect()
        //     urlConn.getContentType()
        new ThreadWrapper() {
            protected void run() throws RippleException
            {
                addGraphPrivate( url, baseURI );
            }
        }.start( Ripple.uriDereferencingTimeout() );
    }


    private void addGraphPrivate( final URL url, final URI baseURI )
        throws RippleException
    {
        s_logger.info( "Importing model " + url.toString() +
            ( ( null == baseURI ) ? "" : " in context " + baseURI.toString() ) );

        boolean verifyData = true;
s_logger.info( "######## dereferencing graph at URL: " + url );

        URLConnection urlConn = openUrlConnection( url );
        InputStream response = null;

        final RDFFormat format = guessRdfFormat( urlConn );
        if ( null == format )
            return;

s_logger.info( "####### Guessed format is " + format.getName() );

        try
        {
            response = urlConn.getInputStream();
        }

        catch ( IOException e )
        {
            throw new RippleException( e );
        }

        try
        {
            if ( null == baseURI )
                repoConnection.add( response, null, format );
            else
                repoConnection.add( response, baseURI.toString(), format, baseURI );
        }

        catch ( Throwable t )
        {
            close( response );

            if ( t instanceof RDFParseException )
            {
                String msg = "line " + ( (RDFParseException) t ).getLineNumber()
                    + ", column " + ( (RDFParseException) t ).getColumnNumber()
                    + ": " + t.getMessage();

                throw new RippleException( msg );
            }

            else
                throw new RippleException( t );
        }

        close( response );

s_logger.info( "####### graph imported without errors" );

        model.touch();
    }

    public void addGraph( final URL url )
        throws RippleException
    {
        String uriStr;

        try
        {
// FIXME: the URL should be treated as a black box.
            uriStr = url.toURI().toString();
        }

        catch ( java.net.URISyntaxException e )
        {
            throw new RippleException( e );
        }

        URI baseURI = createUri( uriStr );
        addGraph( url, baseURI );
    }

    private static void close( InputStream is )
        throws RippleException
    {
        try
        {
            is.close();
        }

        catch ( IOException e )
        {
            throw new RippleException( e );
        }
    }

    public long countStatements( Resource context )
        throws RippleException
    {
        int count = 0;

        try
        {
            RepositoryResult<Statement> stmtIter
                = repoConnection.getStatements(
                    null, null, null, Ripple.useInference(), context );
            while ( stmtIter.hasNext() )
            {
                stmtIter.next();
                count++;
            }

            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return count;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
