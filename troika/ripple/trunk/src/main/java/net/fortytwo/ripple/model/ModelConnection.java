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
import net.fortytwo.ripple.io.RdfSourceAdapter;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.HttpUtils;

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
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFFormat;
import org.openrdf.rio.RDFHandler;
import org.openrdf.rio.RDFParser;
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

private RdfSourceAdapter adapter = null;

public void setSourceAdapter( RdfSourceAdapter adapter )
{
	this.adapter = adapter;
}

	////////////////////////////////////////////////////////////////////////////

	private void constructPrivate( Model model )
		throws RippleException
	{
		this.model = model;

		openRepositoryConnection();

		try
		{
			valueFactory = model.getRepository().getValueFactory();
// s_logger.info( "Opened "
//     + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//     + " (" + openConnections.size() + " total)." );
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
// s_logger.info( "Closing "
//     + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//     + " (" + openConnections.size() + " total)." );

		closeRepositoryConnection( false );

		remove( this );
	}

	/**
	*  Returns the ModelConnection to a normal state after an Exception has
	*  been thrown.
	*/
	public void reset( final boolean rollback )
		throws RippleException
	{
// s_logger.info( "Resetting "
//     + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//     + " (" + openConnections.size() + " total)." );
		closeRepositoryConnection( rollback );
		openRepositoryConnection();
	}

	private static int openRepoConns = 0;

	// Establish a new Sesame connection.
	private void openRepositoryConnection()
		throws RippleException
	{
		try
		{
			repoConnection = model.getRepository().getConnection();
// openRepoConns++;
// s_logger.info( "opened repo connection (making " + openRepoConns + " total): " + repoConnection );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	// Close the current Sesame connection.
	private void closeRepositoryConnection( final boolean rollback )
		throws RippleException
	{
		try
		{
			if ( repoConnection.isOpen() )
			{
				if ( rollback )
					repoConnection.rollback();

// openRepoConns--;
// s_logger.info( "closing repo connection (making " + openRepoConns + " total): " + repoConnection );
				repoConnection.close();

				return;
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		throw new RippleException( "attempting to close already-closed connection" );
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
			reset( true );
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

	public RdfValue findAtLeastOneObject( RdfValue subj, RdfValue pred )
		throws RippleException
	{
		SingleValueSink sink = new SingleValueSink();

		multiply( subj, pred, sink );

		if ( 0 == sink.countReceived() )
			throw new RippleException( "no values resolved for " + pred.toString() + " of " + subj.toString() );
		else
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

			synchronized( repoConnection )
			{
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
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void removeStatementsAbout( final URI subj )
		throws RippleException
	{
		try
		{
			Collection<Statement> stmts = new LinkedList<Statement>();

			synchronized( repoConnection )
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
						subj, null, null, Ripple.useInference() );
				while ( stmtIter.hasNext() )
					stmts.add( stmtIter.next() );
				stmtIter.close();
	
				for ( Iterator<Statement> iter = stmts.iterator(); iter.hasNext(); )
					repoConnection.remove( iter.next() );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
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

			synchronized( repoConnection )
			{
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
		}

		// Warning: the RepositoryResult may be left open.
		catch ( Throwable t )
		{
			reset( true );
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
				synchronized( repoConnection )
				{
					RepositoryResult<Statement> stmtIter
						= repoConnection.getStatements(
//                    subject, null, null, model, includeInferred );
							subjRdf, null, null, Ripple.useInference() );
					while ( stmtIter.hasNext() )
						predicates.add( stmtIter.next().getPredicate() );
					stmtIter.close();
				}
			}

			catch ( Throwable t )
			{
				reset( true );
				throw new RippleException( t );
			}
		}

		Iterator<Value> iter = predicates.iterator();
		while ( iter.hasNext() )
			sink.put( new RdfValue( iter.next() ) );
	}

	////////////////////////////////////////////////////////////////////////////

	// NOTE: not thread-safe on its own
	public void add( Statement st )
		throws RippleException
	{
		try
		{
			repoConnection.add( st );
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void add( RippleValue subj, RippleValue pred, RippleValue obj )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
		URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
		Value objValue = obj.toRdf( this ).getRdfValue();

		try
		{
//            repoConnection.add( subjResource, predUri, obj, singleContext );
			synchronized( repoConnection )
			{
				repoConnection.add( subjResource, predUri, objValue );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
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
			synchronized( repoConnection )
			{
				repoConnection.add( subjResource, predUri, objValue, context );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
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
			synchronized( repoConnection )
			{
				repoConnection.remove( subjResource, predUri, objValue );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void removeStatementsAbout( RdfValue subj, URI context )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );

		try
		{
			synchronized( repoConnection )
			{
				if ( null == context )
					repoConnection.remove( subjResource, null, null );
				else
					repoConnection.remove( subjResource, null, null, context );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
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
			reset( true );
			throw new RippleException( t );
		}
	}

	public RdfValue createValue( final long l )
		throws RippleException
	{
		try
		{
			return new RdfValue(
				valueFactory.createLiteral( "" + l, XMLSchema.LONG ) );
		}

		catch ( Throwable t )
		{
			reset( true );
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
			reset( true );
			throw new RippleException( t );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void setNamespace( final String prefix, final String ns )
		throws RippleException
	{
s_logger.info( "### setting namespace: '" + prefix + "' to " + ns );
		try
		{
			synchronized( repoConnection )
			{
				repoConnection.removeNamespace( prefix );
				repoConnection.setNamespace( prefix, ns );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void setNamespace( final String prefix, final URI ns )
		throws RippleException
	{
		setNamespace( prefix, ns.toString() );
	}

	////////////////////////////////////////////////////////////////////////////

	private URLConnection openUrlConnection( final URL url )
		throws RippleException
	{
		URLConnection urlConn;

		try
		{
			urlConn = url.openConnection();
//HttpUtils.showUrlConnection( urlConn );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		HttpUtils.prepareUrlConnectionForRdfRequest( urlConn );
		HttpUtils.connect( urlConn );

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
		if ( null == adapter )
			throw new RippleException( "no source adapter has been defined" );

		try
		{
// if ( !repoConnection.isOpen() )
// {
// s_logger.info( "connection was found to be closed: " + repoConnection );
// reset( false );
// }
			RDFParser parser = Rio.createParser( format, valueFactory );
			parser.setRDFHandler( adapter );
			synchronized( repoConnection )
			{
				parser.parse( is, context.toString() );
	
				// Commit immediately, so that data is not lost if subsequent
				// operations fail.
				repoConnection.commit();
			}
		}

		catch ( Throwable t )
		{
			reset( true );

			if ( t instanceof org.openrdf.rio.RDFParseException )
			{
				String msg = "line " + ( (org.openrdf.rio.RDFParseException) t ).getLineNumber()
					+ ", column " + ( (org.openrdf.rio.RDFParseException) t ).getColumnNumber()
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

		final RDFFormat format = HttpUtils.guessRdfFormat( urlConn );
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
			synchronized( repoConnection )
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
		}

		catch ( Throwable t )
		{
			reset( true );
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
			synchronized( repoConnection )
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
		}

		catch ( Throwable t )
		{
			reset( true );
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
				synchronized( repoConnection )
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
			}

			catch ( Throwable t )
			{
				reset( true );
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
			reset( true );
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
			reset( true );
			throw new RippleException( t );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	// e.g. CONSTRUCT * FROM {x} p {y}
	public Collection<Statement> serqlQuery( final String queryStr )
		throws RippleException
	{
		Collection<Statement> statements = new ArrayList<Statement>();

		try
		{
			synchronized( repoConnection )
			{
				GraphQueryResult result = repoConnection.prepareGraphQuery(
					QueryLanguage.SERQL, queryStr ).evaluate();
	
				while ( result.hasNext() )
					statements.add( result.next() );
	
				result.close();
			}
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
			RepositoryResult<Statement> stmtIter = null;

			// Perform the query and collect results.
			try
			{
				synchronized( repoConnection )
				{
					stmtIter = repoConnection.getStatements(
						(Resource) rdfSubj, (URI) rdfPred, null, Ripple.useInference() );
				}

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
				try
				{
					stmtIter.close();
				}

				catch ( Throwable t2 )
				{
					System.exit( 1 );
				}

				reset( true );
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

	public void divide( RdfValue obj, RdfValue pred, Sink<RdfValue> sink )
		throws RippleException
	{
		try
		{
			model.getDereferencer().dereference( obj, this );
		}

		catch ( RippleException e )
		{
			// (soft fail)
			s_logger.info( "Failed to dereference URI: " + obj );
		}

		Value rdfObj = obj.getRdfValue();
		Value rdfPred = pred.getRdfValue();

		if ( rdfPred instanceof URI )
		{
			Collection<Value> results = null;
			RepositoryResult<Statement> stmtIter = null;

			// Perform the query and collect results.
			try
			{
				synchronized( repoConnection )
				{
					stmtIter = repoConnection.getStatements(
						null, (URI) rdfPred, rdfObj, Ripple.useInference() );
					while ( stmtIter.hasNext() )
					{
						if ( null == results )
							results = new LinkedList<Value>();
						results.add( stmtIter.next().getSubject() );
					}
					stmtIter.close();
				}
			}

			catch ( Throwable t )
			{
				try
				{
					stmtIter.close();
				}

				catch ( Throwable t2 )
				{
					System.exit( 1 );
				}

				reset( true );
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
