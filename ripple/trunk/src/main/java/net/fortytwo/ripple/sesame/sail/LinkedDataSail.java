package net.fortytwo.ripple.model.sail;

import java.io.File;

import org.openrdf.model.ValueFactory;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailChangedListener;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class LinkedDataSail implements Sail
{
	private Sail localStore;

	public LinkedDataSail( final Sail localStore )
	{
		this.localStore = localStore;
	}

	public void addSailChangedListener( final SailChangedListener listener )
	{
	}

	public SailConnection getConnection()
		throws SailException
	{
		return new LinkedDataSailConnection( localStore );
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
}

// kate: tab-width 4
