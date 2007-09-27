package net.fortytwo.ripple.rdf.sail;

import info.aduna.iteration.CloseableIteration;

import java.util.Iterator;
import java.util.Set;
import java.util.HashSet;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.diff.RdfDiffTee;
import net.fortytwo.ripple.util.Sink;

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

	private RdfDiffSink inputSink, sparqlUpdateSink;

	// Package-level use only.
	LinkedDataSailConnection( final Sail localStore,
									final Dereferencer dereferencer,
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
		inputSink = ( null == listenerSink )
			? adapter
			: new RdfDiffTee( adapter, listenerSink );
sparqlUpdateSink = inputSink;

		open = true;
	}

	// Package-level use only.
	LinkedDataSailConnection( final Sail localStore,
									final Dereferencer dereferencer )
		throws SailException
	{
		this( localStore, dereferencer, null );
	}

// 	public LinkedDataSailConnection( final Sail localStore, final String name )
// 		throws SailException
// 	{
// 		this( localStore );
// 		this.name = name;
// 	}

	public void addConnectionListener( final SailConnectionListener listener )
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
		Sink<Statement> sink = sparqlUpdateSink.adderSink().statementSink();

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
// TODO -- flushes data through the SPARUL pipeline
	}

	public CloseableIteration<? extends BindingSet, QueryEvaluationException> evaluate(
			final TupleExpr tupleExpr,
			final BindingSet bindings,
			final boolean includeInferred )
		throws SailException
	{
		throw new SailException( "this method is not implemented" );
	}

	public CloseableIteration<? extends Resource, SailException> getContextIDs()
		throws SailException
	{
		throw new SailException( "this method is not implemented" );
	}

	public String getNamespace( final String prefix )
		throws SailException
	{
		// Note: only committed namespaces will match.
		return localStoreConnection.getNamespace( prefix );
	}

	public CloseableIteration<? extends Namespace, SailException> getNamespaces()
		throws SailException
	{
		// Note: only committed namespaces will match.
		return localStoreConnection.getNamespaces();
	}

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
		}

		if ( null != obj && obj instanceof URI )
		{
			dereference( (URI) obj );
		}

		// Now that the new RDF data is in the local store, query it.
		return localStoreConnection.getStatements(
			subj, pred, obj, includeInferred, contexts );
	}

	public boolean isOpen()
		throws SailException
	{
		return open;
	}

	public void removeConnectionListener( final SailConnectionListener listener )
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
		Sink<Namespace> sink = sparqlUpdateSink.subtractorSink().namespaceSink();

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
	public void removeStatements( final Resource subj,
									final URI pred,
									final Value obj,
									final Resource... context )
		throws SailException
	{
		Sink<Statement> sink = sparqlUpdateSink.subtractorSink().statementSink();

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
// TODO
	}

	public void setNamespace( final String prefix, final String name )
		throws SailException
	{
		Sink<Namespace> sink = sparqlUpdateSink.adderSink().namespaceSink();

		try
		{
			sink.put( new NamespaceImpl( prefix, name ) );
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}
	}

	public long size( final Resource... contexts )
		throws SailException
	{
		// Number of committed statements.
		return localStoreConnection.size( contexts );
	}

	////////////////////////////////////////////////////////////////////////////

	// Package-level use only.
	void addNamespace( final Namespace ns )
		throws RippleException
	{
		try
		{
			localStoreConnection.setNamespace( ns.getPrefix(), ns.getName() );
		}

		catch ( SailException e )
		{
			throw new RippleException( e );
		}
	}

	// Package-level use only.
	void addStatement( final Statement st )
		throws RippleException
	{
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
		}

		catch ( SailException e )
		{
			throw new RippleException( e );
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

	// Package-level use only.
	void removeNamespace( final Namespace ns )
		throws RippleException
	{
		try
		{
			// Note: removes the namespace with the given prefix,
			// regardless of the associated URI.
			localStoreConnection.removeNamespace( ns.getPrefix() );
		}

		catch ( SailException e )
		{
			throw new RippleException( e );
		}
	}

	// Package-level use only.
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

		catch ( SailException e )
		{
			throw new RippleException( e );
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

	/**
	 * Attempts to return the connection to a normal state after an Exception
	 * has been thrown.
	 */
	private void reset( final boolean rollback ) throws SailException
	{
		closeLocalStoreConnection( rollback );
		openLocalStoreConnection();
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
}

// kate: tab-width 4
