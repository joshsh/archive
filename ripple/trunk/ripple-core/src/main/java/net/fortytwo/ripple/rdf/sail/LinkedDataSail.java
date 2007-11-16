/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import java.io.File;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.util.UrlFactory;

import org.openrdf.model.ValueFactory;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailChangedListener;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;
import org.openrdf.sail.StackableSail;

/**
 * A thread-safe Sail which treats the Semantic Web as a single global graph of
 * linked data.
 */
public class LinkedDataSail implements StackableSail
{
	private Sail baseSail;
	private Dereferencer dereferencer;
	private UrlFactory urlFactory;

	/**
	 * @param baseSail  (should be initialized before this object is used)
	 */
	public LinkedDataSail( final Sail baseSail, final UrlFactory urlFactory )
		throws RippleException
	{
		this.baseSail = baseSail;
		this.urlFactory = urlFactory;

		dereferencer = new HttpUriDereferencer( urlFactory, getValueFactory() );

		// Don't bother trying to dereference terms in these common namespaces.
		dereferencer.addFailureMemo( "http://www.w3.org/XML/1998/namespace#" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema#" );

		// Don't try to dereference the cache index.
		dereferencer.addSuccessMemo( "http://fortytwo.net/2007/08/ripple/cache#" );
	}

	public void addSailChangedListener( final SailChangedListener listener )
	{
	}

	public synchronized SailConnection getConnection()
		throws SailException
	{
		return new LinkedDataSailConnection( baseSail, dereferencer, urlFactory );
	}

	public File getDataDir()
	{
return null;
	}

	public ValueFactory getValueFactory()
	{
		// Inherit the local store's ValueFactory
		return baseSail.getValueFactory();
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

	// Extended API ////////////////////////////////////////////////////////////

	public synchronized LinkedDataSailConnection getConnection( final RdfDiffSink listenerSink )
		throws SailException
	{
		return new LinkedDataSailConnection( baseSail, dereferencer, urlFactory, listenerSink );
	}

public Dereferencer getDereferencer()
{
	return dereferencer;
}

	public Sail getBaseSail()
	{
		return baseSail;
	}
	
	public void setBaseSail( final Sail baseSail )
	{
		this.baseSail = baseSail;
	}
}

// kate: tab-width 4
