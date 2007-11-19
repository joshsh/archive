/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import info.aduna.iteration.CloseableIteration;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.control.Task;
import net.fortytwo.ripple.control.TaskSet;
import net.fortytwo.ripple.rdf.RdfSource;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.NullSource;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;
import net.fortytwo.ripple.util.UniqueFilter;

import org.apache.log4j.Logger;
import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class ModelConnection
{
	private static final Logger LOGGER
		= Logger.getLogger( ModelConnection.class );

	private static Set<ModelConnection> openConnections
		= new LinkedHashSet<ModelConnection>();

//	private static Random rand = new Random();

	private Model model;
	private SailConnection sailConnection;
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

	private synchronized void openSailConnection()
		throws RippleException
	{
		try
		{
			sailConnection = ( null == listenerSink )
				? model.getSail().getConnection()
				: model.getSail().getConnection( listenerSink );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	private synchronized void closeSailConnection( final boolean rollback )
		throws RippleException
	{
		try
		{
			if ( sailConnection.isOpen() )
			{
				if ( rollback )
				{
					sailConnection.rollback();
				}

				sailConnection.close();
			}

			else
			{
				// Don't throw an exception: we could easily end up in a loop.
				LOGGER.error( "tried to close an already-closed connection" );
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

public synchronized SailConnection getSailConnection()
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
//				ModelConnection mc = i.next();
//				mc.close();
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private Resource castToResource( final Value v ) throws RippleException
	{
		if ( v instanceof Resource )
		{
			return (Resource) v;
		}

		else
		{
			throw new RippleException( "value " + v + " is not a Resource" );
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
			throw new RippleException( "value " + v + " is not a Literal" );
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

	public URI uriValue( final RippleValue rv )
		throws RippleException
	{
		Value v = rv.toRdf( this ).getRdfValue();

		if ( v instanceof URI )
		{
			return (URI) v;
		}

		else
		{
			throw new RippleException( "value " + v.toString() + " is not a URI" );
		}
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

// TODO: context handling
	public void copyStatements( final RdfValue src, final RdfValue dest )
		throws RippleException
	{
		final Resource destResource = castToResource( dest.getRdfValue() );

		Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				Resource context = st.getContext();

				try
				{
					if ( null == context )
					{
						sailConnection.addStatement(
							destResource, st.getPredicate(), st.getObject() );
					}

					else
					{
						sailConnection.addStatement(
							destResource, st.getPredicate(), st.getObject(), context );
					}
				}
		
				catch ( Throwable t )
				{
					reset( true );
					throw new RippleException( t );
				}
			}
		};

		getStatements( src, null, null, stSink );
	}

	public void removeStatementsAbout( final URI subj )
		throws RippleException
	{
		try
		{
			sailConnection.removeStatements( subj, null, null );
		}

		catch ( Throwable t )
		{
			reset( true );
			throw new RippleException( t );
		}
	}

	public void putContainerMembers( final RippleValue head, final Sink<RippleValue> sink )
		throws RippleException
	{
		Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				if ( '_' == st.getPredicate().getLocalName().charAt( 0 ) )
				{
					sink.put( new RdfValue( st.getObject() ) );
				}
			}
		};

		getStatements( head.toRdf( this ), null, null, stSink );
	}

	////////////////////////////////////////////////////////////////////////////

	public void findPredicates( final RippleValue subject,
								final Sink<RdfValue> sink )
		throws RippleException
	{
		Sink<Statement> predSelector = new Sink<Statement>()
		{
			Sink<RdfValue> predSink = new UniqueFilter<RdfValue>( sink );

			public void put( final Statement st ) throws RippleException
			{
// TODO: don't create a new RdfValue before checking for uniqueness
				predSink.put( new RdfValue( st.getPredicate() ) );
			}
		};

		getStatements( subject.toRdf( this ), null, null, predSelector );
	}

	////////////////////////////////////////////////////////////////////////////

// FIXME: Statements should be absent from the ModelConnection API
	public void add( final Statement st, final Resource... contexts )
		throws RippleException
	{
		try
		{
			sailConnection.addStatement( st.getSubject(), st.getPredicate(), st.getObject(), contexts );
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
		Value subjValue = subj.toRdf( this ).getRdfValue();
		Value predValue = pred.toRdf( this ).getRdfValue();
		Value objValue = obj.toRdf( this ).getRdfValue();

		if ( !( subjValue instanceof Resource )
				|| !( predValue instanceof URI ) )
		{
			return;
		}

		try
		{
			if ( subjValue instanceof URI )
			{
				sailConnection.addStatement(
					(Resource) subjValue, (URI) predValue, objValue,
					RdfUtils.inferContextUri( (URI) subjValue, valueFactory ) );
			}

			else
			{
				sailConnection.addStatement(
					(Resource) subjValue, (URI) predValue, objValue );
			}
		}

		catch ( SailException e )
		{
			reset( true );
			throw new RippleException( e );
		}
	}

	public void remove( final RippleValue subj, final RippleValue pred, final RippleValue obj )
		throws RippleException
	{
		Value subjValue = subj.toRdf( this ).getRdfValue();
		Value predValue = pred.toRdf( this ).getRdfValue();
		Value objValue = obj.toRdf( this ).getRdfValue();

		if ( !( subjValue instanceof Resource )
				|| !( predValue instanceof URI ) )
		{
			return;
		}

		try
		{
// Does this remove the statement from ALL contexts?
			sailConnection.removeStatements(
				(Resource) subjValue, (URI) predValue, objValue );
		}

		catch ( SailException e )
		{
			reset( true );
			throw new RippleException( e );
		}
	}

// FIXME: URIs should be absent from the ModelConnection API
	public void removeStatementsAbout( final RdfValue subj, final URI context )
		throws RippleException
	{
		Value subjValue = subj.toRdf( this ).getRdfValue();

		if ( !( subjValue instanceof Resource ) )
		{
			return;
		}

		try
		{
			if ( null == context )
			{
				sailConnection.removeStatements( (Resource) subjValue, null, null );
			}

			else
			{
				sailConnection.removeStatements( (Resource) subjValue, null, null, context );
			}
		}

		catch ( SailException e )
		{
			reset( true );
			throw new RippleException( e );
		}
	}

// FIXME: Resources should be absent from the ModelConnection API
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

/*
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
// FIXME: Statements should be absent from the ModelConnection API
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

// FIXME: Statements should be absent from the ModelConnection API
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

// TODO: use CloseableIterationSource
			
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

			private Source<String> comSource = new NullSource<String>();

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

	////////////////////////////////////////////////////////////////////////////

// TODO: Namespaces should not be part of the ModelConnection API
	public void putNamespaces( final Sink<Namespace> sink )
		throws RippleException
	{
		try
		{
			CloseableIteration<? extends Namespace, SailException> iter
				= sailConnection.getNamespaces();

			while ( iter.hasNext() )
			{
				sink.put( iter.next() );
			}
		}

		catch ( SailException e )
		{
			throw new RippleException( e );
		}
	}

	public void putContexts( final Sink<RippleValue> sink )
		throws RippleException
	{
		try
		{
			CloseableIteration<? extends Resource, SailException> iter
				= sailConnection.getContextIDs();

			while ( iter.hasNext() )
			{
				sink.put( new RdfValue( iter.next() ) );
			}
			
			iter.close();
		}

		catch ( SailException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
