package net.fortytwo.ripple.model.impl.sesame;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Set;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.model.LexiconUpdater;
import net.fortytwo.ripple.model.LibraryLoader;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.rdf.sail.LinkedDataSail;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;
import org.openrdf.model.URI;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.sail.Sail;

public class SesameModel implements Model
{
	private static final Logger LOGGER = Logger.getLogger( Model.class );

	private LinkedDataSail sail;
public LinkedDataSail getSail()
{
	return sail;
}

	Set<ModelConnection> openConnections = new LinkedHashSet<ModelConnection>();

	private ModelBridge bridge;
	public ModelBridge getBridge()
	{
		return bridge;
	}

	public SesameModel( final Sail localStore )
		throws RippleException
	{
		LOGGER.debug( "Creating new Model" );
	
		bridge = new ModelBridge();
		UrlFactory urlFactory = new UrlFactory();
		sail = new LinkedDataSail( localStore, urlFactory );

		loadSymbols( urlFactory );
	}

	public Collection<ModelConnection> openConnections()
	{
		Collection<ModelConnection> copy = new LinkedList<ModelConnection>();
		
		synchronized ( openConnections )
		{
			copy.addAll( openConnections );
		}
		
		return copy;
	}

	public void closeOpenConnections() throws RippleException
	{
		synchronized ( openConnections )
		{
			Iterator<ModelConnection> i = openConnections.iterator();
			while ( i.hasNext() )
			{
	//			ModelConnection mc = i.next();
	//			mc.close();
			}
		}
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
		return new SesameModelConnection( this, name, null );
	}

	public ModelConnection getConnection( final String name, final LexiconUpdater updater ) throws RippleException
	{
		return new SesameModelConnection( this, name, updater );
	}

	public Dereferencer getDereferencer()
	{
		return getSail().getDereferencer();
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
