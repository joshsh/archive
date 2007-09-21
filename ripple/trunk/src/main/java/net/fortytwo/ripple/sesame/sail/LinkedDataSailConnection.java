package net.fortytwo.ripple.model.sail;

import info.aduna.iteration.CloseableIteration;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.control.TaskSet;
import net.fortytwo.ripple.io.Dereferencer;

import org.apache.log4j.Logger;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
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

	private static Set<LinkedDataSailConnection> openConnections
		= new LinkedHashSet<LinkedDataSailConnection>();

	private String name = null;
	private boolean open = false;
	private Sail localStore;
	private SailConnection localStoreConnection;
	private Dereferencer dereferencer;

	private TaskSet taskSet = new TaskSet();

	public LinkedDataSailConnection( final Sail localStore,
									 final Dereferencer dereferencer )
		throws SailException
	{
		this.localStore = localStore;
		this.dereferencer = dereferencer;
		openLocalStoreConnection();
		open = true;

		synchronized ( openConnections )
		{
			openConnections.add( this );
		}
	}

// 	public LinkedDataSailConnection( final Sail localStore, final String name )
// 		throws SailException
// 	{
// 		this( localStore );
// 		this.name = name;
// 	}

	public void addConnectionListener( final SailConnectionListener listener )
	{
// TODO
	}

	public void addStatement( final Resource subj,
								final URI pred,
								final Value obj,
								final Resource... contexts )
		throws SailException
	{
// TODO
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

		// Complete any still-executing tasks.
		try
		{
			taskSet.waitUntilEmpty();
		}

		catch ( RippleException e )
		{
			throw new SailException( e );
		}

		closeLocalStoreConnection( false );

		open = false;

		synchronized ( openConnections )
		{
			openConnections.remove( this );
		}
	}

	public void commit()
		throws SailException
	{
// TODO
	}

	public CloseableIteration<? extends BindingSet,QueryEvaluationException> evaluate(
			final TupleExpr tupleExpr,
			final BindingSet bindings,
			final boolean includeInferred )
		throws SailException
	{
		throw new SailException( "this method is not implemented" );
	}

	public CloseableIteration<? extends Resource,SailException> getContextIDs()
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

	public CloseableIteration<? extends Namespace,SailException> getNamespaces()
		throws SailException
	{
		// Note: only committed namespaces will match.
		return localStoreConnection.getNamespaces();
	}

	public CloseableIteration<? extends Statement,SailException> getStatements(
			final Resource subj,
			final URI pred,
			final Value obj,
			final boolean includeInferred,
			final Resource... contexts )
		throws SailException
	{
// TODO: URI dereferencing

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
// TODO
	}

	public void removeNamespace( final String prefix )
		throws SailException
	{
		// Note: only committed namespaces will be affected.
		localStoreConnection.removeNamespace( prefix );
	}

	public void removeStatements( final Resource subj,
									final URI pred,
									final Value obj,
									final Resource... context )
		throws SailException
	{
		// Note: only committed statements will be affected.
		localStoreConnection.removeStatements( subj, pred, obj, context );
	}

	public void rollback()
		throws SailException
	{
// TODO
	}

	public void setNamespace( final String prefix, final String name )
		throws SailException
	{
// TODO
	}

	public long size( final Resource... contexts )
		throws SailException
	{
		// Number of committed statements.
		return localStoreConnection.size( contexts );
	}

	////////////////////////////////////////////////////////////////////////////

	public static List<String> listOpenConnections()
	{
		synchronized ( openConnections )
		{
			List<String> names = new ArrayList<String>( openConnections.size() );

			Iterator<LinkedDataSailConnection> i = openConnections.iterator();
			while ( i.hasNext() )
			{
				names.add( i.next().name );
			}

			return names;
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

	////////////////////////////////////////////////////////////////////////////

/*
	private void dereference( final URI uri )
	{
		try
		{
			dereferencer.dereference( uri, this );
		}

		catch ( RippleException e )
		{
			// (soft fail... don't even log the error)
			return;
		}
	}
*/
}

// kate: tab-width 4
