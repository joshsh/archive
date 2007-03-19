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

import net.fortytwo.ripple.ThreadWrapper;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFParseException;
import org.openrdf.repository.RepositoryResult;

public class ModelConnection
{
    private final static Logger s_logger
        = Logger.getLogger( ModelConnection.class );

    private Model model;
    private RepositoryConnection repoConnection;
    private ValueFactory valueFactory;
    private ModelBridge bridge;
    private String name = null;

    ////////////////////////////////////////////////////////////////////////////

    private void constructPrivate( Model model )
        throws RippleException
    {
        this.model = model;

        try
        {
            repoConnection = model.getRepository().getConnection();
            valueFactory = model.getRepository().getValueFactory();
//System.out.println( "Opened "
//    + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//    + " (" + openConnections + " total)." );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        bridge = model.getBridge();

        add( this );
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

    /**
     *  Returns the ModelConnection to a normal state after an Exception has
     *  been thrown.
     */
    public void reset()
        throws RippleException
    {
        try
        {
            repoConnection.close();
            repoConnection = model.getRepository().getConnection();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
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

    private Resource castToResource( Value v )
        throws RippleException
    {
        if ( v instanceof Resource )
            return (Resource) v;
        else
            throw new RippleException( "value " + v.toString() + " is not a Resource" );
    }

    private URI castToUri( Value v )
        throws RippleException
    {
        if ( v instanceof URI )
            return (URI) v;

        else
            throw new RippleException( "value " + v.toString() + " is not a URI" );
    }

    private Literal castToLiteral( Value v )
        throws RippleException
    {
        if ( v instanceof Literal )
            return (Literal) v;

        else
            throw new RippleException( "value " + v.toString() + " is not a Literal" );
    }

    ////////////////////////////////////////////////////////////////////////////

/*
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

public URI uriValue( RippleValue rv )
{
    Value v = rdfValue( rv ).getRdfValue();
    if ( v instanceof URI )
        return (URI) v;
    else
        throw new RippleException( "value " + v.toString() + " is not a URI" );
}
*/

/*
    private RdfValue rdfValue( RippleValue rv )
        throws RippleException
    {
        if ( v instanceof RdfValue )
            return (RdfValue) rv;
        else
            throw new RippleException( "RippleValue " + rv.toString() + " is not an RdfValue" );
    }
*/

    public boolean booleanValue( RippleValue rv )
        throws RippleException
    {
        Literal l = castToLiteral( rv.toRdf( this ).getRdfValue() );

//        URI type = lit.getDatatype();
//        if ( !type.equals( XMLSchema.BOOLEAN ) )
//            throw new RippleException( "type mismatch: expected " + XMLSchema.BOOLEAN.toString() + ", found " + type.toString() );

        String label = l.getLabel();
// TODO: is capitalization relevant? Can 'true' also be represented as '1'?
        return label.equals( "true" );
    }

    public int intValue( RippleValue rv )
		throws RippleException
    {
        Literal l = castToLiteral( rv.toRdf( this ).getRdfValue() );

//        URI type = l.getDatatype();
//        if ( !type.equals( XMLSchema.INTEGER ) )
//            throw new RippleException( "type mismatch: expected " + XMLSchema.INTEGER.toString() + ", found " + type.toString() );

        try
        {
            return ( new Integer( l.getLabel() ) ).intValue();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public String stringValue( RippleValue v )
        throws RippleException
    {
        Literal l = castToLiteral( v.toRdf( this ).getRdfValue() );

        return l.getLabel();
    }

    public URI uriValue( RippleValue v )
        throws RippleException
    {
        return castToUri( v.toRdf( this ).getRdfValue() );
    }

    /**
     *  A <code>Sink</code> which remembers how many times it has received a
     *  value, as well as the last value received.
     */
    private class SingleValueSink implements Sink<RdfValue>
    {
        RdfValue value = null;
        int valuesReceived = 0;

        public void put( RdfValue v )
            throws RippleException
        {
            value = v;
            valuesReceived++;
        }

        public RdfValue getValue()
        {
            return value;
        }

        public int countReceived()
        {
            return valuesReceived;
        }
    }

    public RdfValue findUniqueProduct( RdfValue subj, RdfValue pred )
        throws RippleException
    {
        SingleValueSink sink = new SingleValueSink();

        model.multiply( subj, pred, sink );

        int count = sink.countReceived();

        if ( 0 == count )
            throw new RippleException( "no values resolved for " + pred.toString() + " of " + subj.toString() );
        else if ( 1 < count )
            throw new RippleException( pred.toString() + " of " + subj.toString() + " resolved to more than one value" );
        else
            return sink.getValue();
    }

    private static RdfValue rdfFirst = new RdfValue( RDF.FIRST );
    private static RdfValue rdfNil = new RdfValue( RDF.NIL );
    private static RdfValue rdfRest = new RdfValue( RDF.REST );

    public List<RippleValue> listValue( final RippleValue listHead )
        throws RippleException
    {
        List<RippleValue> list = new ArrayList<RippleValue>();

        RdfValue cur = listHead.toRdf( this );

        while ( !cur.equals( rdfNil ) )
        {
            RdfValue v = findUniqueProduct( cur, rdfFirst );
            list.add( bridge.get( v ) );
            cur = findUniqueProduct( cur, rdfRest );
        }

        return list;
    }

    // Note: this is a bit of a hack.  Ideally, the Model should handle all RDF queries.
    public Collection<RippleValue> bagValue( final RippleValue head )
        throws RippleException
    {
        Collection<RippleValue> results = new Container();

        try
        {
            boolean useInference = false;

            RepositoryResult<Statement> stmtIter
                = repoConnection.getStatements(
                    castToResource( head.toRdf( this ).getRdfValue() ), null, null, useInference );

            while ( stmtIter.hasNext() )
            {
                Statement st = stmtIter.next();
                if ( '_' == st.getPredicate().getLocalName().charAt( 0 ) )
                    results.add( bridge.get( new RdfValue( st.getObject() ) ) );
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

    public Set<RdfValue> getPredicates( RippleValue subject )
        throws RippleException
    {
        Set<RdfValue> predicates = new HashSet<RdfValue>();
        Resource subjRdf = castToResource( subject.toRdf( this ).getRdfValue() );

        try
        {
            RepositoryResult<Statement> stmtIter
                = repoConnection.getStatements(
//                    subject, null, null, model, includeInferred );
                    subjRdf, null, null, Ripple.useInference() );
            while ( stmtIter.hasNext() )
                predicates.add( new RdfValue( stmtIter.next().getPredicate() ) );
            stmtIter.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return predicates;
    }

    ////////////////////////////////////////////////////////////////////////////

    public void add( RippleValue subj, RippleValue pred, RippleValue obj )
        throws RippleException
    {
        Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
        URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
        Value objValue = obj.toRdf( this ).getRdfValue();

        try
        {
//            repoConnection.add( subjResource, predUri, obj, singleContext );
            repoConnection.add( subjResource, predUri, objValue );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public void add( RippleValue subj, RippleValue pred, RippleValue obj, Resource context )
        throws RippleException
    {
        Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
        URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
        Value objValue = obj.toRdf( this ).getRdfValue();

        try
        {
//            repoConnection.add( subjResource, predUri, obj, singleContext );
            repoConnection.add( subjResource, predUri, objValue, context );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public void remove( RippleValue subj, RippleValue pred, RippleValue obj )
        throws RippleException
    {
        Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
        URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
        Value objValue = obj.toRdf( this ).getRdfValue();

        try
        {
// Does this remove the statement from ALL contexts?
            repoConnection.remove( subjResource, predUri, objValue );
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
            return valueFactory.createURI( s );
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
            return valueFactory.createURI( ns + s );
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
            return valueFactory.createURI( ns.toString() + s );
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
            return valueFactory.createLiteral( s, XMLSchema.STRING );
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
            return valueFactory.createLiteral( s, language );
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
            return valueFactory.createLiteral( s, dataType );
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
            return valueFactory.createLiteral( "" + b, XMLSchema.BOOLEAN );
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
            return valueFactory.createLiteral( "" + i, XMLSchema.INTEGER );
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
            return valueFactory.createLiteral( "" + d, XMLSchema.DOUBLE );
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
            return valueFactory.createBNode();
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
            return valueFactory.createBNode( id );
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

    ////////////////////////////////////////////////////////////////////////////

    public RippleValue createValue( final String s )
        throws RippleException
    {
        try
        {
            return new RdfValue(
                valueFactory.createLiteral( s, XMLSchema.STRING ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public RippleValue createValue( final String s, final String language )
        throws RippleException
    {
        try
        {
            return new RdfValue(
                valueFactory.createLiteral( s, language ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public RippleValue createValue( final String s, final URI dataType )
        throws RippleException
    {
        try
        {
            return new RdfValue(
                valueFactory.createLiteral( s, dataType ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public RippleValue createValue( final boolean b )
        throws RippleException
    {
        try
        {
            return new RdfValue(
                valueFactory.createLiteral( "" + b, XMLSchema.BOOLEAN ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public RippleValue createValue( final int i )
        throws RippleException
    {
        try
        {
            return new RdfValue(
                valueFactory.createLiteral( "" + i, XMLSchema.INTEGER ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public RippleValue createValue( final double d )
        throws RippleException
    {
        try
        {
            return new RdfValue(
                valueFactory.createLiteral( "" + d, XMLSchema.DOUBLE ) );
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
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


    public void addGraph( final URL url, final URI context )
        throws RippleException
    {
        // Wrap the entire operation in the timeout wrapper, as there are various
        // pieces which are capable of hanging:
        //     urlConn.connect()
        //     urlConn.getContentType()
        new ThreadWrapper() {
            protected void run() throws RippleException
            {
                addGraphPrivate( url, context );
            }
        }.start( Ripple.uriDereferencingTimeout() );
    }


    public void addGraph( final InputStream is, final URI context, RDFFormat format )
        throws RippleException
    {
        try
        {
            if ( null == context )
                repoConnection.add( is, null, format );
            else
            {
                String baseUri = context.toString();
                repoConnection.add( is, baseUri, format, context );
            }
        }

        catch ( Throwable t )
        {
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
    }

    private void addGraphPrivate( final URL url, final URI context )
        throws RippleException
    {
        s_logger.info( "Importing model " + url.toString() +
            ( ( null == context ) ? "" : " in context " + context.toString() ) );

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
            addGraph( response, context, format );
        }

        catch ( RippleException e )
        {
            close( response );
            throw e;
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
System.out.println( "########################### Count = " + count );
        return count;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
