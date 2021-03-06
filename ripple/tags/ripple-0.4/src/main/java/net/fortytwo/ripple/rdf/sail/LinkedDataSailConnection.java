/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import info.aduna.iteration.CloseableIteration;

import java.util.Iterator;
import java.util.Set;
import java.util.HashSet;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.rdf.diff.RdfDiffBuffer;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.diff.RdfDiffTee;
import net.fortytwo.ripple.rdf.diff.SynchronizedRdfDiffSink;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.query.BindingSet;
import org.openrdf.query.QueryEvaluationException;
import org.openrdf.query.algebra.TupleExpr;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailConnectionListener;
import org.openrdf.sail.SailException;

/**
 * A thread-safe SailConnection for LinkedDataSail.
 */
public class LinkedDataSailConnection implements SailConnection
{
	private static final Logger LOGGER
		= Logger.getLogger( LinkedDataSailConnection.class );

	private String name = null;
	private boolean open = false;

	private Sail localStore;
	private SailConnection localStoreConnection;
	private ValueFactory valueFactory;
	private Set<SailConnectionListener> listeners = null;

	private Dereferencer dereferencer;
	private SparqlUpdater sparqlUpdater;

	private RdfDiffSink apiInputSink;

	// Buffering input to the wrapped SailConnection avoids deadlocks.
	private RdfDiffBuffer inputBuffer;
	private RdfDiffSink inputSink;

	////////////////////////////////////////////////////////////////////////////

	public synchronized void addConnectionListener( final SailConnectionListener listener )
	{
		if ( null == listeners )
		{
			listeners = new HashSet<SailConnectionListener>();
		}

		listeners.add( listener );
	}

	public synchronized void addStatement( final Resource subj,
								final URI pred,
								final Value obj,
								final Resource... contexts )
		throws SailException
	{
		Sink<Statement> sink = apiInputSink.adderSink().statementSink();

		try
		{
// FIXME: both of these conditions are probably not necessary
			if ( null == contexts || 0 == contexts.length )
			{
				sink.put( valueFactory.createStatement(
					subj, pred, obj ) );
			}

			else
			{
				for ( int i = 0; i < contexts.length; i++ )
				{
					sink.put( valueFactory.createStatement(
						subj, pred, obj, contexts[i] ) );
				}
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public void clear( final Resource... contexts )
		throws SailException
	{
		throw new SailException( "this method is not implemented" );
	}

	public void close()
		throws SailException
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
			sparqlUpdater.flush();
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}

		commitInput();
	}

	public CloseableIteration<? extends BindingSet, QueryEvaluationException> evaluate(
			final TupleExpr tupleExpr,
			final BindingSet bindings,
			final boolean includeInferred )
		throws SailException
	{
		throw new SailException( "this method is not implemented" );
	}

	public synchronized CloseableIteration<? extends Resource, SailException> getContextIDs()
		throws SailException
	{
		try
		{
			return localStoreConnection.getContextIDs();
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	public synchronized String getNamespace( final String prefix )
		throws SailException
	{
		try
		{
			// Note: only committed namespaces will match.
			return localStoreConnection.getNamespace( prefix );
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	public synchronized CloseableIteration<? extends Namespace, SailException> getNamespaces()
		throws SailException
	{
		try
		{
			// Note: only committed namespaces will match.
			return localStoreConnection.getNamespaces();
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
			final Resource... contexts )
		throws SailException
	{
		if ( null != subj && subj instanceof URI )
		{
			dereference( (URI) subj );
			commitInput();
		}

		if ( null != obj && obj instanceof URI )
		{
			dereference( (URI) obj );
			commitInput();
		}

		// Now that the new RDF data is in the local store, query it.
//System.out.println( "getStatements(" + subj + ", " + pred + ", " + obj + ", " + includeInferred + ", " + contexts + ")" );
//System.out.println( "    # contexts = " + contexts.length );
		synchronized ( this )
		{
			try
			{
				return new StatementIteration(
					localStoreConnection.getStatements(
						subj, pred, obj, includeInferred, contexts ) );
			}
	
			catch ( SailException e )
			{
				reset( true );
				throw e;
			}
		}
	}

	public boolean isOpen()
		throws SailException
	{
		return open;
	}

	public synchronized void removeConnectionListener( final SailConnectionListener listener )
	{
		if ( null != listeners )
		{
			listeners.remove( listener );
		}
	}

	// Note: only committed namespaces will be affected.
	public synchronized void removeNamespace( final String prefix )
		throws SailException
	{
		Sink<Namespace> sink = apiInputSink.subtractorSink().namespaceSink();

		try
		{
			// Note: the URI of the Namespace shouldn't matter
			sink.put( new NamespaceImpl( prefix, "" ) );
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	// Note: only committed statements will be affected.
	public synchronized void removeStatements( final Resource subj,
									final URI pred,
									final Value obj,
									final Resource... context )
		throws SailException
	{
		Sink<Statement> sink = apiInputSink.subtractorSink().statementSink();

		try
		{
// FIXME: both of these conditions are probably not necessary
			if ( null == context || 0 == context.length )
			{
				sink.put( valueFactory.createStatement(
					subj, pred, obj ) );
			}

			else
			{
				for ( int i = 0; i < context.length; i++ )
				{
					sink.put( valueFactory.createStatement(
						subj, pred, obj, context[i] ) );
				}
			}
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public void rollback()
		throws SailException
	{
		inputBuffer.clear();
// TODO
	}

	public synchronized void setNamespace( final String prefix, final String name )
		throws SailException
	{
		Sink<Namespace> sink = apiInputSink.adderSink().namespaceSink();

		try
		{
			sink.put( new NamespaceImpl( prefix, name ) );
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public synchronized long size( final Resource... contexts )
		throws SailException
	{
		try
		{
			// Number of committed statements.
			return localStoreConnection.size( contexts );
		}

		catch ( SailException e )
		{
			reset( true );
			throw e;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	LinkedDataSailConnection( final Sail localStore,
									final Dereferencer dereferencer,
									final UrlFactory urlFactory,
									final RdfDiffSink listenerSink )
		throws SailException
	{
		this.localStore = localStore;
		this.dereferencer = dereferencer;

		// Inherit the local store's ValueFactory
		valueFactory = localStore.getValueFactory();

		openLocalStoreConnection();

		SailConnectionOutputAdapter adapter
			= new SailConnectionOutputAdapter( this );
		inputBuffer = new RdfDiffBuffer(
			( null == listenerSink )
				? adapter
				: new RdfDiffTee( adapter, listenerSink ) );
		inputSink = new SynchronizedRdfDiffSink( inputBuffer );

		sparqlUpdater = new SparqlUpdater( urlFactory, inputSink );
		apiInputSink = sparqlUpdater.getSink();

		open = true;
	}

	LinkedDataSailConnection( final Sail localStore,
								final Dereferencer dereferencer,
								final UrlFactory urlFactory )
		throws SailException
	{
		this( localStore, dereferencer, urlFactory, null );
	}

	void addNamespace( final Namespace ns )
		throws RippleException
	{
		try
		{
//localStoreConnection.commit();
			localStoreConnection.setNamespace( ns.getPrefix(), ns.getName() );
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
//System.out.println( "adding statement: " + st );
		Resource context = st.getContext();

		try
		{
			if ( null == context )
			{
				localStoreConnection.addStatement(
					st.getSubject(),
					st.getPredicate(),
					st.getObject() );
			}

			else
			{
				localStoreConnection.addStatement(
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
			// Note: removes the namespace with the given prefix,
			// regardless of the associated URI.
			localStoreConnection.removeNamespace( ns.getPrefix() );
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
			if ( null == context )
			{
				localStoreConnection.removeStatements(
					st.getSubject(),
					st.getPredicate(),
					st.getObject() );
			}

			else
			{
				localStoreConnection.removeStatements(
					st.getSubject(),
					st.getPredicate(),
					st.getObject(),
					context );
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
		localStoreConnection = localStore.getConnection();
	}

	private void closeLocalStoreConnection( final boolean rollback )
		throws SailException
	{
		if ( localStoreConnection.isOpen() )
		{
			if ( rollback )
			{
				localStoreConnection.rollback();
			}

			localStoreConnection.close();
		}

		else
		{
			// Don't throw an exception: we could easily end up in a loop.
			LOGGER.error( "tried to close an already-closed connection" );
		}
	}

	private synchronized void commitInput() throws SailException
	{
		try
		{
			inputBuffer.flush();
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}

		localStoreConnection.commit();
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
		try
		{
			dereferencer.dereference( uri, inputSink.adderSink() );
		}

		catch ( RippleException e )
		{
			// (soft fail... don't even log the error)
			return;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private class StatementIteration implements CloseableIteration<Statement, SailException>
	{
		private SailConnection originalConnection;
		private Statement next = null;
		private CloseableIteration<? extends Statement, SailException> iter;

		public StatementIteration(
			final CloseableIteration<? extends Statement, SailException> iter )
		{
			this.iter = iter;
			originalConnection = localStoreConnection;
		}

		public boolean hasNext() throws SailException
		{
			synchronized ( localStore )
			{
				if ( ok() )
				{
					if ( null != next )
					{
						return true;
					}
	
					else
					{
						if ( iter.hasNext() )
						{
							next = iter.next();
							return true;
						}
	
						else
						{
							return false;
						}
					}
				}
	
				else
				{
					return false;
				}
			}
		}
	
		public Statement next() throws SailException
		{
			synchronized ( localStore )
			{
				if ( null != next )
				{
					Statement tmp = next;
					next = null;
					return tmp;
				}
	
				else if ( ok() )
				{
					return iter.next();
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
			iter.close();
		}

		private boolean ok()
		{
			return originalConnection == localStoreConnection;
		}
	}
}

// kate: tab-width 4
