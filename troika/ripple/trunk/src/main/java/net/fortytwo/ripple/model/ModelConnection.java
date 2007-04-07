package net.fortytwo.ripple.model;

import java.io.InputStream;
import java.io.OutputStream;

import java.net.URL;
import java.net.URLConnection;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Set;
import java.util.Map;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.Random;

import net.fortytwo.ripple.util.ThreadWrapper;
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
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.GraphQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFParseException;
import org.openrdf.rio.rdfxml.util.RDFXMLPrettyWriter;
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
//org.openrdf.rio.Rio.createParser( RDFFormat.TURTLE );

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
//System.out.println( "intValue(" + rv );
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
			reset();
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

	public RdfValue findSingleObject( RdfValue subj, RdfValue pred )
		throws RippleException
	{
		SingleValueSink sink = new SingleValueSink();

		multiply( subj, pred, sink );

		return sink.getValue();
	}

	public RdfValue findAtMostOneObject( RdfValue subj, RdfValue pred )
		throws RippleException
	{
		SingleValueSink sink = new SingleValueSink();

		multiply( subj, pred, sink );

		int count = sink.countReceived();

		if ( 1 < count )
			throw new RippleException( pred.toString() + " of " + subj.toString() + " resolved to more than one value" );
		else
			return sink.getValue();
	}

	public RdfValue findUniqueProduct( RdfValue subj, RdfValue pred )
		throws RippleException
	{
		RdfValue v = findAtMostOneObject( subj, pred );

		if ( null == v )
			throw new RippleException( "no values resolved for " + pred.toString() + " of " + subj.toString() );
		else
			return v;
	}

	// Note: context is ignored.
	// Another note: the source value is not (yet) dereferenced
	public void copyStatements( RdfValue src, RdfValue dest )
		throws RippleException
	{
		Resource srcResource = castToResource( src.getRdfValue() );
		Resource destResource = castToResource( dest.getRdfValue() );

		try
		{
			// Note: 
			Collection<Statement> stmts = new LinkedList<Statement>();
			RepositoryResult<Statement> stmtIter
				= repoConnection.getStatements(
					srcResource, null, null, Ripple.useInference() );
			while ( stmtIter.hasNext() )
				stmts.add( stmtIter.next() );
			stmtIter.close();

			for ( Iterator<Statement> iter = stmts.iterator(); iter.hasNext(); )
			{
				Statement st = iter.next();

				repoConnection.add( destResource, st.getPredicate(), st.getObject() );
//                repoConnection.add( RDF.NIL, RDF.TYPE, RDF.LIST );
			}
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	public void removeStatementsAbout( final URI subj )
		throws RippleException
	{
		try
		{
			Collection<Statement> stmts = new LinkedList<Statement>();
			RepositoryResult<Statement> stmtIter
				= repoConnection.getStatements(
					subj, null, null, Ripple.useInference() );
			while ( stmtIter.hasNext() )
				stmts.add( stmtIter.next() );
			stmtIter.close();

			for ( Iterator<Statement> iter = stmts.iterator(); iter.hasNext(); )
				repoConnection.remove( iter.next() );
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
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
			reset();
			throw new RippleException( t );
		}

		return results;
	}

	////////////////////////////////////////////////////////////////////////////

	public void findPredicates( RippleValue subject, Sink<RdfValue> sink )
		throws RippleException
	{
		Set<Value> predicates = new HashSet<Value>();
		Value v = subject.toRdf( this ).getRdfValue();

		if ( v instanceof Resource )
		{
			Resource subjRdf = (Resource) v;

			try
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
//                    subject, null, null, model, includeInferred );
						subjRdf, null, null, Ripple.useInference() );
				while ( stmtIter.hasNext() )
					predicates.add( stmtIter.next().getPredicate() );
				stmtIter.close();
			}

			catch ( Throwable t )
			{
				reset();
				throw new RippleException( t );
			}
		}

		Iterator<Value> iter = predicates.iterator();
		while ( iter.hasNext() )
			sink.put( new RdfValue( iter.next() ) );
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
			reset();
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
			reset();
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
			reset();
			throw new RippleException( t );
		}
	}

	public void removeStatementsAbout( RdfValue subj, URI context )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );

		try
		{
			if ( null == context )
				repoConnection.remove( subjResource, null, null );
			else
				repoConnection.remove( subjResource, null, null, context );
		}

		catch ( Throwable t )
		{
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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
			reset();
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

	public RdfValue createValue( final String s )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( s, XMLSchema.STRING ) );
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	public RdfValue createValue( final String s, final String language )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( s, language ) );
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	public RdfValue createValue( final String s, final URI dataType )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( s, dataType ) );
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	public RdfValue createValue( final boolean b )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( "" + b, XMLSchema.BOOLEAN ) );
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	public RdfValue createValue( final int i )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( "" + i, XMLSchema.INTEGER ) );
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	public RdfValue createValue( final double d )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( "" + d, XMLSchema.DOUBLE ) );
		}

		catch ( Throwable t )
		{
			reset();
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
			reset();
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
		s_logger.debug( "contentType = " + contentType );

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
		s_logger.debug( "extension = " + ext );

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
			Ripple.getName() + "/" + Ripple.getVersion() );

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
//showUrlConnection( urlConn );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		try
		{
			urlConn.connect();
		}

		catch ( java.io.IOException e )
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
s_logger.debug( "####################### before" );
				repoConnection.add( is, baseUri, format, context );
s_logger.debug( "####################### after" );
			}
		}

		catch ( Throwable t )
		{
			reset();

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

		URLConnection urlConn = openUrlConnection( url );
		InputStream response = null;

		final RDFFormat format = guessRdfFormat( urlConn );
		if ( null == format )
			return;

		s_logger.debug( "guessed format is " + format.getName() );

		try
		{
			response = urlConn.getInputStream();
		}

		catch ( java.io.IOException e )
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
		s_logger.debug( "graph imported without errors" );

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

		catch ( java.io.IOException e )
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
			reset();
			throw new RippleException( t );
		}
		return count;
	}

	////////////////////////////////////////////////////////////////////////////

	// Hackishly find all terms in the given namespace which are the subject
	// of statements.
	private Set<URI> findSubjectsInNamespace( final String ns )
		throws RippleException
	{
		Set<URI> subjects = new HashSet<URI>();

		try
		{
			RepositoryResult<Statement> stmtIter
				= repoConnection.getStatements(
					null, null, null, false );

			while ( stmtIter.hasNext() )
			{
				Resource subj = stmtIter.next().getSubject();
				if ( subj instanceof URI
					&& subj.toString().startsWith( ns ) )
				{
					subjects.add( (URI) subj );
				}
			}

			stmtIter.close();
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}

		return subjects;
	}

	private class SpecialSubgraphHandler implements Sink<Resource>
	{
		private Set<Resource> visited;
		private RDFHandler handler;

		public SpecialSubgraphHandler( final RDFHandler handler )
		{
			this.handler = handler;
			visited = new HashSet<Resource>();
		}

		public void put( Resource r )
			throws RippleException
		{
			if ( visited.contains( r ) )
				return;
			else
				visited.add( r );

			try
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
						r, null, null, false );

				while ( stmtIter.hasNext() )
				{
					Statement st = stmtIter.next();

					// Add all statement about this Resource to the result graph.
					handler.handleStatement( st );

					// Traverse to any neighboring blank nodes (but not to URIs).
					Value obj = st.getObject();
					if ( obj instanceof Resource
						&& !( obj instanceof URI ) )
						put( (Resource) obj );
				}

				stmtIter.close();
			}

			catch ( Throwable t )
			{
				reset();
				throw new RippleException( t );
			}
		}
	}

	public void exportNs( final String nsPrefix, OutputStream out )
		throws RippleException
	{
		Set<URI> subjects = findSubjectsInNamespace( nsPrefix );

		RDFHandler handler;

		try
		{
			handler = new RDFXMLPrettyWriter( out );
			handler.startRDF();
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}

		SpecialSubgraphHandler ssh = new SpecialSubgraphHandler( handler );

		for ( Iterator<URI> subjIter = subjects.iterator(); subjIter.hasNext(); )
			ssh.put( subjIter.next() );

		try
		{
			handler.endRDF();
		}

		catch ( Throwable t )
		{
			reset();
			throw new RippleException( t );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	// e.g. CONSTRUCT * FROM {x} p {y}
	public Collection<Statement> graphQuery( final String queryStr )
		throws RippleException
	{
		Collection<Statement> statements = new ArrayList<Statement>();

		try
		{
			GraphQueryResult result = repoConnection.prepareGraphQuery(
				QueryLanguage.SERQL, queryStr ).evaluate();

			while ( result.hasNext() )
				statements.add( result.next() );

			result.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		return statements;
	}

	////////////////////////////////////////////////////////////////////////////

	public void multiply( RdfValue subj, RdfValue pred, Sink<RdfValue> sink )
		throws RippleException
	{
		try
		{
			model.getDereferencer().dereference( subj, this );
		}

		catch ( RippleException e )
		{
			// (soft fail)
			s_logger.info( "Failed to dereference URI: " + subj );
		}

		Value rdfSubj = subj.getRdfValue();
		Value rdfPred = pred.getRdfValue();

		if ( rdfSubj instanceof Resource && rdfPred instanceof URI )
		{
			// Note: we must collect results in a buffer before putting anything
			//       into the sink, as inefficient as that is, because otherwise
			//       we might end up opening another RepositoryResult before
			//       the one below closes, which currently causes Sesame to
			//       deadlock.  Even using a separate RepositoryConnection for
			//       each RepositoryResult doesn't seem to help.
			Collection<Value> results = null;

			// Perform the query and collect results.
			try
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
						(Resource) rdfSubj, (URI) rdfPred, null, Ripple.useInference() );
				while ( stmtIter.hasNext() )
				{
					if ( null == results )
						results = new LinkedList<Value>();
					results.add( stmtIter.next().getObject() );
				}
				stmtIter.close();
			}

			catch ( Throwable t )
			{
				reset();
				throw new RippleException( t );
			}

			// Now copy any results from the buffer into the sink.
			if ( null != results )
			{
				for ( Iterator<Value> resultIter = results.iterator();
					resultIter.hasNext(); )
				{
					sink.put( new RdfValue( resultIter.next() ) );
				}
			}
		}
	}
}

// kate: tab-width 4
