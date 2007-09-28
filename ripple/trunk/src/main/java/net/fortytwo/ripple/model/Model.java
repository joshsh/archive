/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.util.Collection;
import java.util.LinkedList;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.rdf.sail.LinkedDataSail;
import net.fortytwo.ripple.rdf.sail.LinkedDataSailConnection;
import net.fortytwo.ripple.util.LibraryLoader;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Namespace;
import org.openrdf.sail.Sail;


public class Model
{
	private static final Logger LOGGER = Logger.getLogger( Model.class );

	private String name;
	private LinkedDataSail sail;
public LinkedDataSail getSail()
{
	return sail;
}

	private ModelBridge bridge;
	public ModelBridge getBridge()
	{
		return bridge;
	}

	public Model( final Sail localStore, final String name )
		throws RippleException
	{
		LOGGER.debug( "Creating new Model '" + name + "'" );
	
		this.name = name;
		bridge = new ModelBridge();
		UrlFactory urlFactory = new UrlFactory();
		sail = new LinkedDataSail( localStore, urlFactory );

		loadSymbols( urlFactory );
	}

	void loadSymbols( final UrlFactory uf )
		throws RippleException
	{
		ModelConnection mc = getConnection( "for Model.loadSymbols" );

		// At the moment, op needs to be a special value for the sake of the
		// evaluator.  This has the side-effect of making it a keyword.
		bridge.add( Operator.OP, mc );

		LibraryLoader loader = new LibraryLoader();

		try
		{
			loader.load( uf, mc );
		}

		catch ( RippleException e )
		{
			mc.close();
			throw e;
		}

		mc.close();
	}

	public ModelConnection getConnection( final String name )
		throws RippleException
	{
		return new ModelConnection( this, name, null );
	}

	////////////////////////////////////////////////////////////////////////////

	// Note: this may be a very expensive operation (see Sesame API).
	public long countStatements()
		throws RippleException
	{
return 0;
/* TODO
		long size;

		try
		{
			RepositoryConnection rc = repository.getConnection();
			size = rc.size();
			rc.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		return size;
*/
	}

	public void showNamespaces()
		throws RippleException
	{
/* TODO
		try
		{
			RepositoryConnection conn = repository.getConnection();

			RepositoryResult<Namespace> nsIter
				= conn.getNamespaces();

			int maxlen = 0;

			while ( nsIter.hasNext() )
			{
				Namespace ns = nsIter.next();
				int len = ns.getPrefix().length();

				if ( len > maxlen )
				{
					maxlen = len;
				}
			}

			nsIter.close();

			nsIter = conn.getNamespaces();
			while ( nsIter.hasNext() )
			{
				Namespace ns = nsIter.next();
				String prefix = ns.getPrefix();
				int len = prefix.length();
				System.out.print( prefix + ":" );

				for ( int i = 0; i < maxlen - len + 2; i++ )
				{
					System.out.print( " " );
				}

				System.out.println( ns.getName() );
			}
			nsIter.close();

			conn.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
*/
	}

	public Collection<RippleValue> getContexts()
		throws RippleException
	{
		Collection<RippleValue> contexts = new LinkedList<RippleValue>();

/* TODO
		try
		{
			RepositoryConnection conn = repository.getConnection();

			RepositoryResult<Resource> contextIter
				= conn.getContextIDs();

			while ( contextIter.hasNext() )
			{
				contexts.add( new RdfValue( contextIter.next() ) );
			}

			contextIter.close();
			conn.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
*/
		return contexts;
	}
}

// kate: tab-width 4
