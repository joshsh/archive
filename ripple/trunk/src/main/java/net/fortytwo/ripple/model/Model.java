/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import info.aduna.iteration.CloseableIteration;

import java.util.Collection;
import java.util.LinkedList;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.rdf.sail.LinkedDataSail;
import net.fortytwo.ripple.rdf.sail.LinkedDataSailConnection;
import net.fortytwo.ripple.util.LibraryLoader;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Namespace;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;


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

	private void loadSymbols( final UrlFactory uf )
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
}

// kate: tab-width 4
