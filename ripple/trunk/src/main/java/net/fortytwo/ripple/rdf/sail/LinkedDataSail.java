package net.fortytwo.ripple.rdf.sail;

import java.io.File;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.io.HttpUriDereferencer;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.ValueFactory;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailChangedListener;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class LinkedDataSail implements Sail
{
	private static final Logger LOGGER = Logger.getLogger( LinkedDataSail.class );

	private Sail localStore;
	private Dereferencer dereferencer;

	/**
	 * @param localStore  (should be initialized before this object is used)
	 */
	public LinkedDataSail( final Sail localStore )
		throws RippleException
	{
		this.localStore = localStore;

		UrlFactory urlFactory = new UrlFactory();
		dereferencer = new HttpUriDereferencer( urlFactory );

		// Don't bother trying to dereference terms in these common namespaces.
		dereferencer.addFailureMemo( "http://www.w3.org/XML/1998/namespace#" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema#" );

		// Don't try to dereference the cache index.
		dereferencer.addSuccessMemo( Ripple.getCacheUri() );
	}

	public void addSailChangedListener( final SailChangedListener listener )
	{
	}

	public SailConnection getConnection()
		throws SailException
	{
		return new LinkedDataSailConnection( localStore, dereferencer );
	}

	public File getDataDir()
	{
return null;
	}

	public ValueFactory getValueFactory()
	{
return null;
	}

	public void initialize()
		throws SailException
	{
	}

	public boolean isWritable()
		throws SailException
	{
		return true;
	}

	public void removeSailChangedListener( final SailChangedListener listener )
	{
	}

	public void setDataDir( final File dataDir )
	{
	}

	// Deprecated.
	public void setParameter( final String key, final String value )
	{
	}

	public void shutDown()
		throws SailException
	{
	}

	////////////////////////////////////////////////////////////////////////////

	public SailConnection getConnection( final RdfDiffSink updateSink )
		throws SailException
	{
		return new LinkedDataSailConnection( localStore, dereferencer, updateSink );
	}

// 	public Dereferencer getDereferencer()
// 	{
// 		return dereferencer;
// 	}
}

// kate: tab-width 4
