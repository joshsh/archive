/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.io.OutputStream;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.Random;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfNullSink;
import net.fortytwo.ripple.io.RdfSink;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.control.Task;
import net.fortytwo.ripple.control.TaskSet;

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
import org.openrdf.repository.RepositoryResult;
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFHandler;

public class ModelConnection
{
	private static final Logger LOGGER
		= Logger.getLogger( ModelConnection.class );

	private static int openRepoConns = 0;

	private static Set<ModelConnection> openConnections
		= new LinkedHashSet<ModelConnection>();

	private static final RdfValue
		RDF_FIRST = new RdfValue( RDF.FIRST ),
		RDF_NIL = new RdfValue( RDF.NIL ),
		RDF_REST = new RdfValue( RDF.REST );

	private static Random rn = new Random();

	private TaskSet taskSet = new TaskSet();

	private Model model;
	private RepositoryConnection repoConnection;
	private ValueFactory valueFactory;
public ValueFactory getValueFactory()
{
	return valueFactory;
}
	private ModelBridge bridge;
	private String name = null;

private RdfSink rdfSink = new RdfNullSink();
public RdfSink getRdfSink()
{
	return rdfSink;
}
public void setRdfSink( final RdfSink sink )
{
	rdfSink = sink;
}

	////////////////////////////////////////////////////////////////////////////

	public ModelConnection( final Model model )
		throws RippleException
	{
		this.model = model;

		openRepositoryConnection();

		try
		{
			valueFactory = model.getRepository().getValueFactory();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		bridge = model.getBridge();

		add( this );
	}

	public ModelConnection( final Model model, final String name )
		throws RippleException
	{
		this( model );
		this.name = name;
	}

	public Model getModel()
	{
		return model;
	}

	public RepositoryConnection getRepositoryConnection()
	{
		return repoConnection;
	}

	public void close() throws RippleException
	{
// LOGGER.info( "Closing "
// 	+ ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
// 	+ " (" + openConnections.size() + " total)." );

		// Complete any still-executing tasks.
		taskSet.waitUntilEmpty();

		closeRepositoryConnection( false );

		remove( this );
	}

	/**
	*  Returns the ModelConnection to a normal state after an Exception has
	*  been thrown.
	*/
	public void reset( final boolean rollback ) throws RippleException
	{
// LOGGER.info( "Resetting "
//     + ( ( null == name ) ? "anonymous connection" : "connection \"" + name + "\"" )
//     + " (" + openConnections.size() + " total)." );
		closeRepositoryConnection( rollback );
		openRepositoryConnection();
	}

	// Establish a new Sesame connection.
	private void openRepositoryConnection()
		throws RippleException
	{
		try
		{
			repoConnection = model.getRepository().getConnection();
// openRepoConns++;
// LOGGER.info( "opened repo connection (making " + openRepoConns + " total): " + repoConnection );
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
				{
					repoConnection.rollback();
				}

// openRepoConns--;
// LOGGER.info( "closing repo connection (making " + openRepoConns + " total): " + repoConnection );
				repoConnection.close();

				return;
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		// Don't throw an exception: we could easily end up in a loop.
		LOGGER.error( "tried to close an already-closed connection" );
	}

	////////////////////////////////////////////////////////////////////////////

	private static void add( final ModelConnection mc )
	{
		synchronized ( openConnections )
		{
			openConnections.add( mc );
		}
	}

	private static void remove( final ModelConnection mc )
	{
		synchronized ( openConnections )
		{
			openConnections.remove( mc );
		}
	}

	public static List<String> listOpenConnections()
	{
		synchronized ( openConnections )
		{
			List<String> names = new ArrayList<String>( openConnections.size() );

			Iterator<ModelConnection> i = openConnections.iterator();
			while ( i.hasNext() )
			{
				names.add( i.next().name );
			}

			return names;
		}
	}

	public static void closeOpenConnections() throws RippleException
	{
		synchronized ( openConnections )
		{
			Iterator<ModelConnection> i = openConnections.iterator();
			while ( i.hasNext() )
			{
				ModelConnection mc = i.next();
//				mc.close();
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	Resource castToResource( final Value v ) throws RippleException
	{
		if ( v instanceof Resource )
		{
			return (Resource) v;
		}

		else
		{
			throw new RippleException( "value " + v.toString() + " is not a Resource" );
		}
	}

	private URI castToUri( final Value v ) throws RippleException
	{
		if ( v instanceof URI )
		{
			return (URI) v;
		}

		else
		{
			throw new RippleException( "value " + v.toString() + " is not a URI" );
		}
	}

	private Literal castToLiteral( final Value v ) throws RippleException
	{
		if ( v instanceof Literal )
		{
			return (Literal) v;
		}

		else
		{
			throw new RippleException( "value " + v.toString() + " is not a Literal" );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public boolean booleanValue( final RippleValue rv ) throws RippleException
	{
		Literal l = castToLiteral( rv.toRdf( this ).getRdfValue() );

//        URI type = lit.getDatatype();
//        if ( !type.equals( XMLSchema.BOOLEAN ) )
//            throw new RippleException( "type mismatch: expected " + XMLSchema.BOOLEAN.toString() + ", found " + type.toString() );

		String label = l.getLabel();
// TODO: is capitalization relevant? Can 'true' also be represented as '1'?
		return label.equals( "true" );
	}

	public NumericLiteral numericValue( final RippleValue rv )
		throws RippleException
	{
		if ( rv instanceof NumericLiteral )
		{
			return (NumericLiteral) rv;
		}

		else
		{
			return new NumericLiteral( rv.toRdf( this ) );
		}
	}

	public int intValue( final RippleValue rv ) throws RippleException
	{
		if ( rv instanceof NumericLiteral )
		{
			// Note: possible loss of precision
			return ( (NumericLiteral) rv ).intValue();
		}

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

	public String stringValue( final RippleValue v ) throws RippleException
	{
		Literal l = castToLiteral( v.toRdf( this ).getRdfValue() );

		return l.getLabel();
	}

	public URI uriValue( final RippleValue v )
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
		private RdfValue value = null;
		private int valuesReceived = 0;

		public void put( final RdfValue v ) throws RippleException
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

	public RdfValue findSingleObject( final RdfValue subj, final RdfValue pred )
		throws RippleException
	{
		SingleValueSink sink = new SingleValueSink();

		multiply( subj, pred, sink );

		return sink.getValue();
	}

	public RdfValue findAtLeastOneObject( final RdfValue subj, final RdfValue pred )
		throws RippleException
	{
		SingleValueSink sink = new SingleValueSink();

		multiply( subj, pred, sink );

		if ( 0 == sink.countReceived() )
		{
			throw new RippleException( "no values resolved for " + pred.toString() + " of " + subj.toString() );
		}

		else
		{
			return sink.getValue();
		}
	}

	public RdfValue findAtMostOneObject( final RdfValue subj, final RdfValue pred )
		throws RippleException
	{
		SingleValueSink sink = new SingleValueSink();

		multiply( subj, pred, sink );

		int count = sink.countReceived();

		if ( 1 < count )
		{
			throw new RippleException( pred.toString() + " of " + subj.toString() + " resolved to more than one value" );
		}

		else
		{
			return sink.getValue();
		}
	}

	public RdfValue findUniqueProduct( final RdfValue subj, final RdfValue pred )
		throws RippleException
	{
		RdfValue v = findAtMostOneObject( subj, pred );

		if ( null == v )
		{
			throw new RippleException( "no values resolved for " + pred.toString() + " of " + subj.toString() );
		}

		else
		{
			return v;
		}
	}

	// Note: context is ignored.
	// Another note: the source value is not (yet) dereferenced
	public void copyStatements( final RdfValue src, final RdfValue dest )
		throws RippleException
	{
		Resource srcResource = castToResource( src.getRdfValue() );
		Resource destResource = castToResource( dest.getRdfValue() );

		try
		{
			// Note: 
			Collection<Statement> stmts = new LinkedList<Statement>();

			synchronized ( repoConnection )
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
						srcResource, null, null, Ripple.useInference() );

				while ( stmtIter.hasNext() )
				{
					stmts.add( stmtIter.next() );
				}

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

			synchronized ( repoConnection )
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
						subj, null, null, Ripple.useInference() );

				while ( stmtIter.hasNext() )
				{
					stmts.add( stmtIter.next() );
				}

				stmtIter.close();
	
				for ( Iterator<Statement> iter = stmts.iterator(); iter.hasNext(); )
				{
					repoConnection.remove( iter.next() );
				}
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public List<RippleValue> listValue( final RippleValue listHead )
		throws RippleException
	{
		List<RippleValue> list = new ArrayList<RippleValue>();

		RdfValue cur = listHead.toRdf( this );

		while ( !cur.equals( RDF_NIL ) )
		{
			RdfValue v = findUniqueProduct( cur, RDF_FIRST );
			list.add( bridge.get( v ) );
			cur = findUniqueProduct( cur, RDF_REST );
		}

		return list;
	}

	// Note: this is a bit of a hack.  Ideally, the Model should handle all RDF queries.
	public Collection<RippleValue> bagValue( final RippleValue head )
		throws RippleException
	{
		Collection<RippleValue> results = new LinkedList<RippleValue>();

		try
		{
			boolean useInference = false;

			synchronized ( repoConnection )
			{
				RepositoryResult<Statement> stmtIter
					= repoConnection.getStatements(
						castToResource( head.toRdf( this ).getRdfValue() ), null, null, useInference );
	
				while ( stmtIter.hasNext() )
				{
					Statement st = stmtIter.next();
					if ( '_' == st.getPredicate().getLocalName().charAt( 0 ) )
					{
						results.add( bridge.get( new RdfValue( st.getObject() ) ) );
					}
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

	public void findPredicates( final RippleValue subject,
								final Sink<RdfValue> sink )
		throws RippleException
	{
		Set<Value> predicates = new HashSet<Value>();
		Value v = subject.toRdf( this ).getRdfValue();

		if ( v instanceof Resource )
		{
			Resource subjRdf = (Resource) v;

			try
			{
				synchronized ( repoConnection )
				{
					RepositoryResult<Statement> stmtIter
						= repoConnection.getStatements(
//                    subject, null, null, model, includeInferred );
							subjRdf, null, null, Ripple.useInference() );

					while ( stmtIter.hasNext() )
					{
						predicates.add( stmtIter.next().getPredicate() );
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

		Iterator<Value> iter = predicates.iterator();
		while ( iter.hasNext() )
		{
			sink.put( new RdfValue( iter.next() ) );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	// NOTE: not thread-safe on its own
	public void add( final Statement st, final Resource... contexts )
		throws RippleException
	{
		try
		{
			repoConnection.add( st, contexts );
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void add( final RippleValue subj, final RippleValue pred, final RippleValue obj )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
		URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
		Value objValue = obj.toRdf( this ).getRdfValue();

		try
		{
//            repoConnection.add( subjResource, predUri, obj, singleContext );
			synchronized ( repoConnection )
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

	public void add( final RippleValue subj, final RippleValue pred, final RippleValue obj, final Resource context )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
		URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
		Value objValue = obj.toRdf( this ).getRdfValue();

		try
		{
//            repoConnection.add( subjResource, predUri, obj, singleContext );
			synchronized ( repoConnection )
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

	public void remove( final RippleValue subj, final RippleValue pred, final RippleValue obj )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
		URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
		Value objValue = obj.toRdf( this ).getRdfValue();

		try
		{
// Does this remove the statement from ALL contexts?
			synchronized ( repoConnection )
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

	public void removeStatementsAbout( final RdfValue subj, final URI context )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );

		try
		{
			synchronized ( repoConnection )
			{
				if ( null == context )
				{
					repoConnection.remove( subjResource, null, null );
				}

				else
				{
					repoConnection.remove( subjResource, null, null, context );
				}
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public long countStatements( final Resource context )
			throws RippleException
	{
		int count = 0;

		try
		{
			synchronized ( repoConnection )
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

	public URI createUri( final String s ) throws RippleException
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

	public BNode createBNode() throws RippleException
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

	public BNode createBNode( final String id ) throws RippleException
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

	private static int randomInt( final int lo, final int hi )
	{
		int n = hi - lo + 1;
		int i = rn.nextInt() % n;

		if (i < 0)
		{
			i = -i;
		}

		return lo + i;
	}

/*
	URI createRandomUri() throws RippleException
	{
		return createUri( "urn:random:" + randomInt( 0, Integer.MAX_VALUE ) );
	}
*/

	////////////////////////////////////////////////////////////////////////////

	public RdfValue createValue( final String s ) throws RippleException
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

	public void setNamespace( final String prefix, final String ns, final boolean override )
		throws RippleException
	{
//LOGGER.info( "### setting namespace: '" + prefix + "' to " + ns );
		try
		{
			synchronized ( repoConnection )
			{
				if ( override || null == repoConnection.getNamespace( prefix ) )
				{
					repoConnection.removeNamespace( prefix );
					repoConnection.setNamespace( prefix, ns );
				}
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void setNamespace( final String prefix, final URI ns, final boolean override )
		throws RippleException
	{
		setNamespace( prefix, ns.toString(), override );
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
			synchronized ( repoConnection )
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

		public void put( final Resource r )
			throws RippleException
		{
			if ( visited.contains( r ) )
			{
				return;
			}

			else
			{
				visited.add( r );
			}

			try
			{
				synchronized ( repoConnection )
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
						{
							put( (Resource) obj );
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
		}
	}

	public void exportNs( final String nsPrefix, final OutputStream out )
		throws RippleException
	{
		Set<URI> subjects = findSubjectsInNamespace( nsPrefix );

		RDFHandler handler = Rio.createWriter( Ripple.exportFormat(), out );

		try
		{
			handler.startRDF();
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}

		SpecialSubgraphHandler ssh = new SpecialSubgraphHandler( handler );

		for ( Iterator<URI> subjIter = subjects.iterator(); subjIter.hasNext(); )
		{
			ssh.put( subjIter.next() );
		}

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
			synchronized ( repoConnection )
			{
				GraphQueryResult result = repoConnection.prepareGraphQuery(
					QueryLanguage.SERQL, queryStr ).evaluate();

				while ( result.hasNext() )
				{
					statements.add( result.next() );
				}

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

	private void dereference( final RdfValue v )
	{
		try
		{
			model.getDereferencer().dereference( v, this );
		}

		catch ( RippleException e )
		{
			// (soft fail... don't even log the error)
			return;
		}
	}

	private class MultiplyTask extends Task
	{
		private RdfValue subj, pred;
		private Sink<RdfValue> sink;

		public MultiplyTask( final RdfValue subj,
							final RdfValue pred,
							final Sink<RdfValue> sink )
		{
			this.subj = subj;
			this.pred = pred;
			this.sink = sink;
		}

		public void executeProtected() throws RippleException
		{
			multiply( subj, pred, sink );
		}

		protected void stopProtected()
		{
			synchronized ( sink )
			{
				sink = new NullSink<RdfValue>();
			}
		}
	}

	public void multiplyAsynch( final RdfValue subj, final RdfValue pred, final Sink<RdfValue> sink )
		throws RippleException
	{
		MultiplyTask task = new MultiplyTask( subj, pred, sink );
		taskSet.add( task );
	}

	public void getStatements( final RdfValue subj,
								final RdfValue pred,
								final RdfValue obj,
								final Sink<Statement> sink )
		throws RippleException
	{
		if ( null != subj )
		{
			dereference( subj );
		}

		if ( null != obj )
		{
			dereference( obj );
		}

		Value rdfSubj = ( null == subj ) ? null : subj.getRdfValue();
		Value rdfPred = ( null == pred ) ? null : pred.getRdfValue();
		Value rdfObj = ( null == obj ) ? null : obj.getRdfValue();

		if ( ( null == rdfSubj || rdfSubj instanceof Resource )
				&& ( null == rdfPred || rdfPred instanceof URI ) )
		{
			// Note: we must collect results in a buffer before putting anything
			//       into the sink, as inefficient as that is, because otherwise
			//       we might end up opening another RepositoryResult before
			//       the one below closes, which currently causes Sesame to
			//       deadlock.  Even using a separate RepositoryConnection for
			//       each RepositoryResult doesn't seem to help.
			Buffer<Statement> buffer = new Buffer<Statement>( sink );
			RepositoryResult<Statement> stmtIter = null;

			// Perform the query and collect results.
			try
			{
				synchronized ( repoConnection )
				{
					stmtIter = repoConnection.getStatements(
						(Resource) rdfSubj, (URI) rdfPred, rdfObj, Ripple.useInference() );
stmtIter.enableDuplicateFilter();
				}

				while ( stmtIter.hasNext() )
				{
					buffer.put( stmtIter.next() );
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

			buffer.flush();
		}
	}

	public void multiply( final RdfValue subj, final RdfValue pred, final Sink<RdfValue> sink )
		throws RippleException
	{
		Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				sink.put( new RdfValue( st.getObject() ) );
			}
		};

		getStatements( subj, pred, null, stSink );
	}

	public void divide( final RdfValue obj, final RdfValue pred, final Sink<RdfValue> sink )
		throws RippleException
	{
		Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				sink.put( new RdfValue( st.getSubject() ) );
			}
		};

		getStatements( null, pred, obj, stSink );
	}
}

// kate: tab-width 4
