/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import info.aduna.iteration.CloseableIteration;

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
import net.fortytwo.ripple.rdf.RdfNullSink;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfSource;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.sail.LinkedDataSailConnection;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.NullSource;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;
import net.fortytwo.ripple.control.Task;
import net.fortytwo.ripple.control.TaskSet;

import org.apache.log4j.Logger;

import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.GraphQueryResult;
import org.openrdf.rio.Rio;
import org.openrdf.rio.RDFHandler;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class ModelConnection
{
	private static final Logger LOGGER
		= Logger.getLogger( ModelConnection.class );

	private static Set<ModelConnection> openConnections
		= new LinkedHashSet<ModelConnection>();

	private static Random rand = new Random();

	private Model model;
	private LinkedDataSailConnection sailConnection;
	private RdfDiffSink listenerSink;
	private ValueFactory valueFactory;
	private String name = null;

	private TaskSet taskSet = new TaskSet();

	////////////////////////////////////////////////////////////////////////////

	public ModelConnection( final Model model, final String name, final RdfDiffSink listenerSink )
		throws RippleException
	{
		this.model = model;
		this.name = name;
		this.listenerSink = listenerSink;

		try
		{
			valueFactory = model.getSail().getValueFactory();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		openSailConnection();

		synchronized ( openConnections )
		{
			openConnections.add( this );
		}
	}

	public Model getModel()
	{
		return model;
	}

	public void close() throws RippleException
	{
		// Complete any still-executing tasks.
		taskSet.waitUntilEmpty();

		closeSailConnection( false );

		synchronized ( openConnections )
		{
			openConnections.remove( this );
		}
	}

	/**
	*  Returns the ModelConnection to a normal state after an Exception has
	*  been thrown.
	*/
	public void reset( final boolean rollback ) throws RippleException
	{
		closeSailConnection( rollback );
		openSailConnection();
	}

	public void commit() throws RippleException
	{
		try
		{
			sailConnection.commit();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	// Establish a new Sesame connection.
	private void openSailConnection()
		throws RippleException
	{
		try
		{
			synchronized ( model )
			{
				sailConnection = ( null == listenerSink )
					? (LinkedDataSailConnection) model.getSail().getConnection()
					: model.getSail().getConnection( listenerSink );
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	// Close the current Sesame connection.
	private void closeSailConnection( final boolean rollback )
		throws RippleException
	{
		try
		{
			synchronized ( model )
			{
				if ( sailConnection.isOpen() )
				{
					if ( rollback )
					{
						sailConnection.rollback();
					}
	
					sailConnection.close();
	
					return;
				}
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		// Don't throw an exception: we could easily end up in a loop.
		LOGGER.error( "tried to close an already-closed connection" );
	}

public SailConnection getSailConnection()
{
	return sailConnection;
}

	////////////////////////////////////////////////////////////////////////////

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
/* TODO
		Resource srcResource = castToResource( src.getRdfValue() );
		Resource destResource = castToResource( dest.getRdfValue() );

		try
		{
			// Note: 
			Collection<Statement> stmts = new LinkedList<Statement>();

			synchronized ( sailConnection )
			{
				RepositoryResult<Statement> stmtIter
					= sailConnection.getStatements(
						srcResource, null, null, Ripple.useInference() );

				while ( stmtIter.hasNext() )
				{
					stmts.add( stmtIter.next() );
				}

				stmtIter.close();

				for ( Iterator<Statement> iter = stmts.iterator(); iter.hasNext(); )
				{
					Statement st = iter.next();
	
					sailConnection.add( destResource, st.getPredicate(), st.getObject() );
//                sailConnection.add( RDF.NIL, RDF.TYPE, RDF.LIST );
				}
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
*/
	}

	public void removeStatementsAbout( final URI subj )
		throws RippleException
	{
/* TODO
		try
		{
			Collection<Statement> stmts = new LinkedList<Statement>();

			synchronized ( sailConnection )
			{
				RepositoryResult<Statement> stmtIter
					= sailConnection.getStatements(
						subj, null, null, Ripple.useInference() );

				while ( stmtIter.hasNext() )
				{
					stmts.add( stmtIter.next() );
				}

				stmtIter.close();
	
				for ( Iterator<Statement> iter = stmts.iterator(); iter.hasNext(); )
				{
					sailConnection.remove( iter.next() );
				}
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
*/
	}

	// Note: this is a bit of a hack.  Ideally, the Model should handle all RDF queries.
	public Collection<RippleValue> bagValue( final RippleValue head )
		throws RippleException
	{
		Collection<RippleValue> results = new LinkedList<RippleValue>();
/* TODO
		try
		{
			boolean useInference = false;

			synchronized ( sailConnection )
			{
				RepositoryResult<Statement> stmtIter
					= sailConnection.getStatements(
						castToResource( head.toRdf( this ).getRdfValue() ), null, null, useInference );
	
				while ( stmtIter.hasNext() )
				{
					Statement st = stmtIter.next();
					if ( '_' == st.getPredicate().getLocalName().charAt( 0 ) )
					{
						results.add( model.getBridge().get( new RdfValue( st.getObject() ) ) );
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
*/

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
				//synchronized ( model )
				{
					CloseableIteration<? extends Statement, SailException> stmtIter
						= sailConnection.getStatements(
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

	public void add( final Statement st, final Resource... contexts )
		throws RippleException
	{
		try
		{
			//synchronized ( model )
			{
				sailConnection.addStatement( st.getSubject(), st.getPredicate(), st.getObject(), contexts );
			}
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void add( final RippleValue subj, final RippleValue pred, final RippleValue obj, final Resource... contexts )
		throws RippleException
	{
		Resource subjResource = castToResource( subj.toRdf( this ).getRdfValue() );
		URI predUri = castToUri( pred.toRdf( this ).getRdfValue() );
		Value objValue = obj.toRdf( this ).getRdfValue();

		try
		{
			//synchronized ( model )
			{
				sailConnection.addStatement( subjResource, predUri, objValue, contexts );
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
			//synchronized ( model )
			{
				sailConnection.removeStatements( subjResource, predUri, objValue );
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
			//synchronized ( model )
			{
				if ( null == context )
				{
					sailConnection.removeStatements( subjResource, null, null );
				}

				else
				{
					sailConnection.removeStatements( subjResource, null, null, context );
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
			//synchronized ( model )
			{
				CloseableIteration<? extends Statement, SailException> stmtIter
					= sailConnection.getStatements(
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
		int i = rand.nextInt() % n;

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

	public Statement createStatement( final Resource subj, final URI pred, final Value obj )
		throws RippleException
	{
		try
		{
			return valueFactory.createStatement( subj, pred, obj );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

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
			//synchronized ( model )
			{
				if ( override || null == sailConnection.getNamespace( prefix ) )
				{
					sailConnection.removeNamespace( prefix );
					sailConnection.setNamespace( prefix, ns );
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

	// e.g. CONSTRUCT * FROM {x} p {y}
	public Collection<Statement> serqlQuery( final String queryStr )
		throws RippleException
	{
		Collection<Statement> statements = new ArrayList<Statement>();
/* TODO
		try
		{
			synchronized ( sailConnection )
			{
				GraphQueryResult result = sailConnection.prepareGraphQuery(
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
*/
		return statements;
	}

	////////////////////////////////////////////////////////////////////////////

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

	public void getNamespaces( final Sink<Namespace> sink )
		throws RippleException
	{
		CloseableIteration<? extends Namespace, SailException> nsIter = null;

		Buffer<Namespace> buffer = new Buffer<Namespace>( sink );

		try
		{
			//synchronized ( model )
			{
				nsIter = sailConnection.getNamespaces();
			}

			while ( nsIter.hasNext() )
			{
				buffer.put( nsIter.next() );
			}

			nsIter.close();
		}

		catch ( Throwable t )
		{
			try
			{
				nsIter.close();
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

	public void getStatements( final RdfValue subj,
								final RdfValue pred,
								final RdfValue obj,
								final Sink<Statement> sink )
		throws RippleException
	{
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
			CloseableIteration<? extends Statement, SailException> stmtIter = null;

			// Perform the query and collect results.
			try
			{
				//synchronized ( model )
				{
					stmtIter = sailConnection.getStatements(
						(Resource) rdfSubj, (URI) rdfPred, rdfObj, Ripple.useInference() );
//stmtIter.enableDuplicateFilter();

					while ( stmtIter.hasNext() )
					{
						buffer.put( stmtIter.next() );
					}
	
					stmtIter.close();
				}
			}

			catch ( Throwable t )
			{
				try
				{
					if ( null != stmtIter )
					{
						stmtIter.close();
					}
				}

				catch ( Throwable t2 )
				{
					t2.printStackTrace( System.err );
					System.exit( 1 );
				}

				reset( true );
				throw new RippleException( t );
			}

			buffer.flush();
		}
	}

	public RdfSource getSource()
	{
		return new RdfSource()
		{
			private Source<Statement> stSource = new Source<Statement>()
			{
				public void writeTo( final Sink<Statement> sink )
					throws RippleException
				{
					getStatements( null, null, null, sink );
				}
			};

			private Source<Namespace> nsSource = new Source<Namespace>()
			{
				public void writeTo( final Sink<Namespace> sink )
					throws RippleException
				{
					getNamespaces( sink );
				}
			};

			private Source comSource = new NullSource<String>();

			public Source<Statement> statementSource()
			{
				return stSource;
			}

			public Source<Namespace> namespaceSource()
			{
				return nsSource;
			}

			public Source<String> commentSource()
			{
				return comSource;
			}
		};
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
