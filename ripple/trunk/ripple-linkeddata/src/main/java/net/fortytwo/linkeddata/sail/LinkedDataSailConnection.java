/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.linkeddata.sail;

import info.aduna.iteration.CloseableIteration;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.diff.RdfDiffBuffer;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.diff.RdfDiffTee;
import net.fortytwo.ripple.rdf.diff.SynchronizedRdfDiffSink;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.URIMap;
import org.apache.log4j.Logger;
import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.query.BindingSet;
import org.openrdf.query.Dataset;
import org.openrdf.query.QueryEvaluationException;
import org.openrdf.query.algebra.TupleExpr;
import org.openrdf.query.algebra.evaluation.TripleSource;
import org.openrdf.query.algebra.evaluation.impl.EvaluationStrategyImpl;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailConnectionListener;
import org.openrdf.sail.SailException;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 * A thread-safe SailConnection for LinkedDataSail.
 * External objects which are assumed to be thread-safe:
 * - the base Sail's ValueFactory
 * - any SailConnectionListeners added to this SailConnection
 * - the UriDereferencer
 * External objects which are not assumed to be thread-safe:
 * - the base Sail and its SailConnections.  All operations involving the base
 *   Sail or its connections are synchronized w.r.t. the base Sail
 */
// TODO: implement commit() and rollback() properly
// TODO: data synchronization between the base Sail and the Web
// TODO: non-blocking / multithreaded SPARUL
// TODO: cut down on excessive synchronization
public class LinkedDataSailConnection implements SailConnection
{
	private static final Logger LOGGER
		= Logger.getLogger( LinkedDataSailConnection.class );

	private boolean open = false;

	private Sail baseSail;
	private SailConnection baseConnection;
	private ValueFactory valueFactory;
	private Set<SailConnectionListener> listeners = null;

	private WebClosure webClosure;
	
	// Note: SparqlUpdater is not thread-safe, so we must synchronize all
	//       operations involving it.
	private SparqlUpdater sparqlUpdater;

	private RdfDiffSink apiInputSink;

	// Buffering input to the wrapped SailConnection avoids deadlocks.
	private RdfDiffBuffer baseSailWriteBuffer;
	private RdfDiffSink baseSailWriteSink;

	private boolean dereferenceSubject = true;
	private boolean dereferencePredicate = false;
	private boolean dereferenceObject = false;
	
	////////////////////////////////////////////////////////////////////////////

	// Connection listener methods are synchronized w.r.t. this SailConnection.
	public synchronized void addConnectionListener( final SailConnectionListener listener )
	{
		if ( null == listeners )
		{
			listeners = new HashSet<SailConnectionListener>();
		}

		listeners.add( listener );
	}

	public void addStatement( final Resource subj,
							final URI pred,
							final Value obj,
							final Resource... contexts )
		throws SailException
	{
		Sink<Statement, RippleException> sink = apiInputSink.adderSink().statementSink();

		try
		{
// FIXME: both of these conditions are probably not necessary
			if ( null == contexts || 0 == contexts.length )
			{
				synchronized ( sparqlUpdater )
				{
					sink.put( valueFactory.createStatement(
						subj, pred, obj ) );
				}
			}

			else
			{
				synchronized ( sparqlUpdater )
				{				
					for ( int i = 0; i < contexts.length; i++ )
					{
						sink.put( valueFactory.createStatement(
							subj, pred, obj, contexts[i] ) );
					}
				}
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public void clear( final Resource... contexts )	throws SailException
	{
// TODO: notify the dereferencer.
		synchronized ( baseSail )
		{
			baseConnection.clear( contexts );
		}
	}

	public void clearNamespaces() throws SailException
	{
		synchronized ( baseSail )
		{
			baseConnection.clearNamespaces();
		}
	}

	public synchronized void close() throws SailException
	{
		commit();

		closeLocalStoreConnection( false );

		open = false;
	}

	public void commit()
		throws SailException
	{
		try
		{
			// Note: currently this will block until all SPARUL requests have
			//       been pushed.
			synchronized ( sparqlUpdater )
			{
				sparqlUpdater.flush();
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}

		commitInput();
	}

	// Note: doesn't need to be synchronized, as it reduces to getStatements
	//       calls, which are thread-safe and return thread-safe objects.
	public CloseableIteration<? extends BindingSet, QueryEvaluationException> evaluate(
			final TupleExpr tupleExpr,
			final Dataset dataset,
			final BindingSet bindings,
			final boolean includeInferred )
		throws SailException
	{
		// Decompose queries into getStatements so we can dereference URIs.
		try
		{
			TripleSource tripleSource = new SailConnectionTripleSource( this, valueFactory, includeInferred );
			EvaluationStrategyImpl strategy = new EvaluationStrategyImpl( tripleSource, dataset );

			return strategy.evaluate( tupleExpr, bindings );
		}
		
		catch ( QueryEvaluationException e )
		{
			throw new SailException( e );
		}
	}

	public CloseableIteration<? extends Resource, SailException> getContextIDs()
		throws SailException
	{
		try
		{
			CloseableIteration<? extends Resource, SailException> iter;
			
			synchronized ( baseSail )
			{
				iter = baseConnection.getContextIDs();
			}
			
			return new ThreadSafeIteration<Resource>( iter );
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	public String getNamespace( final String prefix )
		throws SailException
	{
		try
		{			
			synchronized ( baseSail )
			{
				// Note: only committed namespaces will match.
				return baseConnection.getNamespace( prefix );
			}
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	public CloseableIteration<? extends Namespace, SailException> getNamespaces()
		throws SailException
	{
		try
		{
			CloseableIteration<? extends Namespace, SailException> iter;

			synchronized ( baseSail )
			{
				// Note: only committed namespaces will match.
				iter = baseConnection.getNamespaces();
			}
			
			return new ThreadSafeIteration<Namespace>( iter );
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

// Note: not sychronized, on account of URI dereferencing
	public CloseableIteration<? extends Statement, SailException> getStatements(
			final Resource subj,
			final URI pred,
			final Value obj,
			final boolean includeInferred,
			final Resource... contexts ) throws SailException
	{
		extendClosure( subj, pred, obj );
			
		// Now that the new RDF data is in the local store, query it.
//System.out.println( "getStatements(" + subj + ", " + pred + ", " + obj + ", " + includeInferred + ", " + contexts + ")" );
//System.out.println( "    # contexts = " + contexts.length );
		try
		{
			CloseableIteration<? extends Statement, SailException> iter;
			
			synchronized ( baseSail )
			{
				iter = baseConnection.getStatements(
					subj, pred, obj, includeInferred, contexts );
			}
			
			return new ThreadSafeIteration<Statement>( iter );
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	public boolean isOpen() throws SailException
	{
		return open;
	}

	// Connection listener methods are synchronized w.r.t. this SailConnection.
	public synchronized void removeConnectionListener( final SailConnectionListener listener )
	{
		if ( null != listeners )
		{
			listeners.remove( listener );
		}
	}

	// Note: only committed namespaces will be affected.
	public void removeNamespace( final String prefix )
		throws SailException
	{
		Sink<Namespace, RippleException> sink = apiInputSink.subtractorSink().namespaceSink();

		try
		{
			synchronized ( sparqlUpdater )
			{
				// Note: the URI of the Namespace shouldn't matter
				sink.put( new NamespaceImpl( prefix, "" ) );
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	// Note: only committed statements will be affected.
	public void removeStatements( final Resource subj,
									final URI pred,
									final Value obj,
									final Resource... context )
		throws SailException
	{
		Sink<Statement, RippleException> sink = apiInputSink.subtractorSink().statementSink();

		try
		{
// FIXME: both of these conditions are probably not necessary
			if ( null == context || 0 == context.length )
			{
				synchronized ( sparqlUpdater )
				{
					sink.put( valueFactory.createStatement(
						subj, pred, obj ) );
				}
			}

			else
			{
				synchronized ( sparqlUpdater )
				{
					for ( int i = 0; i < context.length; i++ )
					{
						sink.put( valueFactory.createStatement(
							subj, pred, obj, context[i] ) );
					}
				}
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public void rollback() throws SailException
	{
		synchronized ( baseSailWriteBuffer )
		{
			baseSailWriteBuffer.clear();
		}
	}

	public void setNamespace( final String prefix, final String name )
		throws SailException
	{
		Sink<Namespace, RippleException> sink = apiInputSink.adderSink().namespaceSink();

		try
		{
			synchronized ( sparqlUpdater )
			{
				sink.put( new NamespaceImpl( prefix, name ) );
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public long size( final Resource... contexts ) throws SailException
	{
		try
		{
			synchronized ( baseSail )
			{
				// Number of committed statements.
				return baseConnection.size( contexts );
			}
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	LinkedDataSailConnection( final Sail baseSail,
								final WebClosure webClosure,
								final URIMap URIMap,
								final RdfDiffSink listenerSink )
		throws SailException
	{
		this.baseSail = baseSail;
		this.webClosure = webClosure;

		// Inherit the local store's ValueFactory
		valueFactory = baseSail.getValueFactory();

		openLocalStoreConnection();

		SailConnectionOutputAdapter adapter
			= new SailConnectionOutputAdapter( this );
		baseSailWriteBuffer = new RdfDiffBuffer(
			( null == listenerSink )
				? adapter
				: new RdfDiffTee( adapter, listenerSink ) );
		baseSailWriteSink = new SynchronizedRdfDiffSink( baseSailWriteBuffer, baseSailWriteBuffer );

		sparqlUpdater = new SparqlUpdater( URIMap, baseSailWriteSink );
		apiInputSink = sparqlUpdater.getSink();

		open = true;
	}

	LinkedDataSailConnection( final Sail localStore,
								final WebClosure webClosure,
								final URIMap URIMap )
		throws SailException
	{
		this( localStore, webClosure, URIMap, null );
	}

	void addNamespace( final Namespace ns )
		throws RippleException
	{
		try
		{
			synchronized ( baseSail )
			{
				baseConnection.setNamespace( ns.getPrefix(), ns.getName() );
			}
		}

		catch ( Throwable t )
		{
			resetR( true );
			throw new RippleException( t );
		}
	}

	void addStatement( final Statement st )
		throws RippleException
	{
//System.out.println( "Adding statement: " + st );
//System.out.println( "    subject = " + st.getSubject() );
//System.out.println( "    predicate = " + st.getPredicate() );
//System.out.println( "    object = " + st.getObject() );
//System.out.println( "    context = " + st.getContext() );
		Resource context = st.getContext();

		try
		{
			if ( null == context )
			{
				baseConnection.addStatement(
					st.getSubject(),
					st.getPredicate(),
					st.getObject() );
			}

			else
			{
				baseConnection.addStatement(
					st.getSubject(),
					st.getPredicate(),
					st.getObject(),
					context );
			}

			if ( null != listeners )
			{
				Iterator<SailConnectionListener> iter = listeners.iterator();
				while ( iter.hasNext() )
				{
					iter.next().statementAdded( st );
				}
			}
		}

		catch ( Throwable t )
		{
			resetR( true );
			throw new RippleException( t );
		}
	}

	void removeNamespace( final Namespace ns )
		throws RippleException
	{
		try
		{
			synchronized ( baseSail )
			{
				// Note: removes the namespace with the given prefix,
				// regardless of the associated URI.
				baseConnection.removeNamespace( ns.getPrefix() );
			}
		}

		catch ( Throwable t )
		{
			resetR( true );
			throw new RippleException( t );
		}
	}

	void removeStatement( final Statement st )
		throws RippleException
	{
		Resource context = st.getContext();

		try
		{
			synchronized ( baseSail )
			{
				if ( null == context )
				{
					baseConnection.removeStatements(
						st.getSubject(),
						st.getPredicate(),
						st.getObject() );
				}

				else
				{
					baseConnection.removeStatements(
						st.getSubject(),
						st.getPredicate(),
						st.getObject(),
						context );
				}
			}
		}

		catch ( Throwable t )
		{
			resetR( true );
			throw new RippleException( t );
		}

		if ( null != listeners )
		{
			Iterator<SailConnectionListener> iter = listeners.iterator();
			while ( iter.hasNext() )
			{
				iter.next().statementRemoved( st );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private void openLocalStoreConnection()
		throws SailException
	{
		synchronized ( baseSail )
		{
			baseConnection = baseSail.getConnection();
		}
	}

	private void closeLocalStoreConnection( final boolean rollback )
		throws SailException
	{
		synchronized ( baseSail )
		{
			if ( baseConnection.isOpen() )
			{
				if ( rollback )
				{
					baseConnection.rollback();
				}
	
				baseConnection.close();
			}
	
			else
			{
				// Don't throw an exception: we could easily end up in a loop.
				LOGGER.error( "tried to close an already-closed connection" );
			}
		}
	}

	private synchronized void commitInput() throws SailException
	{
		try
		{
			baseSailWriteBuffer.flush();
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}

		baseConnection.commit();
	}

	/**
	 * Attempts to return the connection to a normal state after an Exception
	 * has been thrown.
	 */
	private void reset( final boolean rollback ) throws SailException
	{
		closeLocalStoreConnection( rollback );
		openLocalStoreConnection();
	}

	private void resetR( final boolean rollback ) throws RippleException
	{
		try
		{
			reset( rollback );
		}

		catch ( SailException e )
		{
			throw new RippleException( e );
		}
	}

	private void dereference( final URI uri )
	{
//System.out.println( "dereferencing URI: " + uri );
		try
		{
			webClosure.extend( uri, baseSailWriteSink.adderSink() );
		}

		catch ( RippleException e )
		{
			//if ( LinkedDataSail.logFailedUris() )
			//{
				e.logError();
			//}
		}
	}

	private void extendClosure( final Resource subj, final URI pred, final Value obj ) throws SailException
	{
		boolean changed = false;
		
		if ( dereferenceSubject && null != subj && subj instanceof URI )
		{
			dereference( (URI) subj );
			changed = true;
		}

		if ( dereferencePredicate && null != subj )
		{
			dereference( pred );
			changed = true;
		}
		
		if ( dereferenceObject && null != obj && obj instanceof URI )
		{
			dereference( (URI) obj );
			changed = true;
		}

		if ( changed )
		{
			commitInput();
		}
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * A CloseableIteration which is thread-safe with respect to the base Sail
	 * and halts if this SailConnection is closed or throws an Exception.
	 */
	private class ThreadSafeIteration<T> implements CloseableIteration<T, SailException>
	{
		private SailConnection originalConnection;
		private T next = null;
		private CloseableIteration<? extends T, SailException> wrappedIteration;

		public ThreadSafeIteration(
			final CloseableIteration<? extends T, SailException> iter )
		{
			this.wrappedIteration = iter;
			originalConnection = baseConnection;
		}

		public boolean hasNext() throws SailException
		{
			synchronized ( baseSail )
			{
				if ( !ok() )
				{
					return false;
				}
				
				if ( null != next )
				{
					return true;
				}

				if ( !wrappedIteration.hasNext() )
				{
					return false;
				}
				
				next = wrappedIteration.next();
				return true;
			}
		}
	
		public T next() throws SailException
		{
			synchronized ( baseSail )
			{
				if ( null != next )
				{
					T tmp = next;
					next = null;
					return tmp;
				}
	
				else if ( ok() )
				{
					return wrappedIteration.next();
				}
	
				else
				{
					throw new SailException( "iterator has no next element" );
				}
			}
		}
	
		public void remove() throws SailException
		{
// TODO
		}

		public void close() throws SailException
		{
			originalConnection = null;
			wrappedIteration.close();
		}

		private boolean ok()
		{
			return originalConnection == baseConnection;
		}
	}	
}

// kate: tab-width 4
