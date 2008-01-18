/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import info.aduna.iteration.CloseableIteration;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.RippleProperties;
import net.fortytwo.ripple.io.WebClosure;
import net.fortytwo.ripple.io.FileUriDereferencer;
import net.fortytwo.ripple.io.HttpUriDereferencer;
import net.fortytwo.ripple.io.JarUriDereferencer;
import net.fortytwo.ripple.io.VerbatimRdfizer;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.util.UrlFactory;
import org.apache.log4j.Logger;
import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.rio.RDFFormat;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailChangedListener;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;
import org.openrdf.sail.StackableSail;

import java.io.File;
import java.util.Iterator;

/**
 * A thread-safe Sail which treats the Semantic Web as a single global graph of
 * linked data.
 */
public class LinkedDataSail implements StackableSail
{
	private static final String LOG_FAILED_URIS = "net.fortytwo.ripple.rdf.sail.logFailedUris";

	private static final Logger LOGGER = Logger.getLogger( LinkedDataSail.class );

	private RippleProperties properties;
	private static boolean logFailedUris;
	
	private URI
		rplCacheRoot,
		rplCacheSuccessMemo,
		rplCacheFailureMemo;
	
	private Sail baseSail;
	private WebClosure webClosure;
	private UrlFactory urlFactory;
	
	private boolean initialized = false;
	
	/**
	 * @param baseSail  (should be initialized before this object is used)
	 */
	public LinkedDataSail( final Sail baseSail, final UrlFactory urlFactory )
		throws RippleException
	{
		if (null == properties)
		{
			properties = Ripple.getProperties();
			logFailedUris = properties.getBoolean( LOG_FAILED_URIS );
		}
		
		this.baseSail = baseSail;
		this.urlFactory = urlFactory;

		webClosure = createDefaultClosureManager();
	}

	public void addSailChangedListener( final SailChangedListener listener )
	{
	}

	public synchronized SailConnection getConnection()
		throws SailException
	{
		if ( !initialized )
		{
			throw new SailException( "LinkedDataSail has not been initialized" );
		}
		
		return new LinkedDataSailConnection( baseSail, webClosure, urlFactory );
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

	public void initialize() throws SailException
	{
		ValueFactory vf = getValueFactory();
		
		rplCacheRoot = vf.createURI( Ripple.getCacheUri() );
		rplCacheSuccessMemo = vf.createURI(
			"http://fortytwo.net/2007/08/ripple/cache#successMemo" );
		rplCacheFailureMemo = vf.createURI(
			"http://fortytwo.net/2007/08/ripple/cache#failureMemo" );
		
		restoreCacheMetaData();
		
		initialized = true;
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

	public void shutDown() throws SailException
	{
		persistCacheMetadata();
	}

	// Extended API ////////////////////////////////////////////////////////////

	public synchronized LinkedDataSailConnection getConnection( final RdfDiffSink listenerSink )
		throws SailException
	{
		return new LinkedDataSailConnection( baseSail, webClosure, urlFactory, listenerSink );
	}

public WebClosure getClosureManager()
{
	return webClosure;
}

	public Sail getBaseSail()
	{
		return baseSail;
	}
	
	public void setBaseSail( final Sail baseSail )
	{
		this.baseSail = baseSail;
	}
	
	////////////////////////////////////////////////////////////////////////////

	private WebClosure createDefaultClosureManager()
	{
		WebClosure cm = new WebClosure( urlFactory, getValueFactory() );

		// Add URI dereferencers.
		cm.addDereferencer( "http", new HttpUriDereferencer() );
		cm.addDereferencer( "jar", new JarUriDereferencer() );
		cm.addDereferencer( "file", new FileUriDereferencer() );

		// Add rdfizers.
		cm.addRdfizer( RdfUtils.findMediaType( RDFFormat.RDFXML ), new VerbatimRdfizer( RDFFormat.RDFXML ) );
		cm.addRdfizer( RdfUtils.findMediaType( RDFFormat.NTRIPLES ), new VerbatimRdfizer( RDFFormat.NTRIPLES ) );
		cm.addRdfizer( RdfUtils.findMediaType( RDFFormat.TURTLE ), new VerbatimRdfizer( RDFFormat.TURTLE ) );
		cm.addRdfizer( RdfUtils.findMediaType( RDFFormat.TRIG ), new VerbatimRdfizer( RDFFormat.TRIG ) );
		cm.addRdfizer( RdfUtils.findMediaType( RDFFormat.TRIX ), new VerbatimRdfizer( RDFFormat.TRIX ) );
		cm.addRdfizer( RdfUtils.findMediaType( RDFFormat.N3 ), new VerbatimRdfizer( RDFFormat.N3 ) );


		// Don't bother trying to dereference terms in these common namespaces.
		cm.addFailureMemo( "http://www.w3.org/XML/1998/namespace#" );
		cm.addFailureMemo( "http://www.w3.org/2001/XMLSchema" );
		cm.addFailureMemo( "http://www.w3.org/2001/XMLSchema#" );

		// Don't try to dereference the cache index.
		cm.addSuccessMemo( "http://fortytwo.net/2007/08/ripple/cache#" );

		return cm;
	}

	/**
	 * Writes cache metadata to the base Sail.
	 * Note: for now, this metadata resides in the null context.
	 */
	private void persistCacheMetadata()	throws SailException
	{
		ValueFactory vf = getValueFactory();
		SailConnection sc = baseSail.getConnection();
		
		// Clear any existing cache metadata (in any named graph).
		sc.removeStatements( rplCacheRoot, null, null );
		sc.commit();

		LOGGER.debug( "writing success memos" );
		for ( Iterator<String> iter = webClosure.getSuccessMemos().iterator(); iter.hasNext(); )
		{
			sc.addStatement( rplCacheRoot, rplCacheSuccessMemo, vf.createLiteral( iter.next() ) );
		}

		LOGGER.debug( "writing failure memos" );
		for ( Iterator<String> iter = webClosure.getFailureMemos().iterator(); iter.hasNext(); )
		{
			sc.addStatement( rplCacheRoot, rplCacheFailureMemo, vf.createLiteral( iter.next() ) );
		}

		sc.commit();
		sc.close();
	}

	/**
	 * Restores dereferencer state by reading success and failure memos from
	 * the last session (if present).
	 */
	private void restoreCacheMetaData() throws SailException
	{
		CloseableIteration<? extends Statement, SailException> iter;
		SailConnection sc = baseSail.getConnection();
		
		// Read success memos.
		iter = sc.getStatements( rplCacheRoot, rplCacheSuccessMemo, null, false );
		while ( iter.hasNext() )
		{
			Value obj = iter.next().getObject();
			if ( obj instanceof Literal )
			{
				webClosure.addSuccessMemo( ( (Literal) obj ).getLabel() );
			}
		}
		
		// Read failure memos.
		iter = sc.getStatements( rplCacheRoot, rplCacheFailureMemo, null, false );
		while ( iter.hasNext() )
		{
			Value obj = iter.next().getObject();
			if ( obj instanceof Literal )
			{
				webClosure.addFailureMemo( ( (Literal) obj ).getLabel() );
			}
		}		
		
		sc.close();
	}
	
	public static boolean logFailedUris()
	{	
		return logFailedUris;
	}
}

// kate: tab-width 4
