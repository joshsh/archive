/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.io.OutputStream;

import java.net.URL;

import java.util.Iterator;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

import org.openrdf.rio.RDFFormat;

public final class CacheManager
{
	private static final Logger LOGGER = Logger.getLogger( CacheManager.class );

	private static RdfValue
		rplCacheRoot,
		rplCacheSuccessMemo,
		rplCacheFailureMemo;

	private static boolean initialized = false;

	private CacheManager()
	{
	}

	private static void initialize( final ModelConnection mc )
		throws RippleException
	{
		rplCacheRoot = new RdfValue( mc.createUri(
			Ripple.getCacheUri() ) );
		rplCacheSuccessMemo = new RdfValue( mc.createUri(
			"http://fortytwo.net/2007/08/ripple/cache#successMemo" ) );
		rplCacheFailureMemo = new RdfValue( mc.createUri(
			"http://fortytwo.net/2007/08/ripple/cache#failureMemo" ) );

		initialized = true;
	}

	public static RDFFormat loadCache( final URL url,
								RDFFormat format,
								final ModelConnection mc )
		throws RippleException
	{
		if ( !initialized )
		{
			initialize( mc );
		}

		RdfImporter importer = new RdfImporter( mc );
		SesameInputAdapter sc = new SesameInputAdapter( importer );

		format = ( null == format )
			? RdfUtils.read( url, sc, url.toString() )
			: RdfUtils.read( url, sc, url.toString(), format );

		restoreCacheMetaData( mc );

		return format;
	}

	public static void writeCacheTo( final OutputStream out,
									final RDFFormat format,
									final ModelConnection mc )
		throws RippleException
	{
		if ( !initialized )
		{
			initialize( mc );
		}

		persistCacheMetadata( mc );

		RdfUtils.write( mc.getModel().getRepository(), out, format );
	}

	////////////////////////////////////////////////////////////////////////////

	/**
	 * Writes cache metadata to the triple store.
	 * Note: for now, this metadata resides in the null context.
	 */
	private static void persistCacheMetadata( final ModelConnection mc )
		throws RippleException
	{
		Dereferencer dereferencer = mc.getModel().getDereferencer();

		mc.removeStatementsAbout( rplCacheRoot, null );

		LOGGER.debug( "writing success memos" );
		for ( Iterator<String> iter = dereferencer.getSuccessMemos().iterator(); iter.hasNext(); )
		{
			mc.add( rplCacheRoot, rplCacheSuccessMemo, mc.createValue( iter.next() ) );
		}

		LOGGER.debug( "writing failure memos" );
		for ( Iterator<String> iter = dereferencer.getFailureMemos().iterator(); iter.hasNext(); )
		{
			mc.add( rplCacheRoot, rplCacheFailureMemo, mc.createValue( iter.next() ) );
		}
	}

	/**
	 * Restores dereferencer state by reading success and failure memos from
	 * the last session (if present).
	 */
	private static void restoreCacheMetaData( final ModelConnection mc )
		throws RippleException
	{
		final Dereferencer dereferencer = mc.getModel().getDereferencer();

		Sink<RdfValue> successMemoSink = new Sink<RdfValue>()
		{
			public void put( final RdfValue v ) throws RippleException
			{
				dereferencer.addSuccessMemo( mc.stringValue( v ) );
			}
		};

		Sink<RdfValue> failureMemoSink = new Sink<RdfValue>()
		{
			public void put( final RdfValue v ) throws RippleException
			{
				dereferencer.addFailureMemo( mc.stringValue( v ) );
			}
		};

		mc.multiply( rplCacheRoot, rplCacheSuccessMemo, successMemoSink );
		mc.multiply( rplCacheRoot, rplCacheFailureMemo, failureMemoSink );
	}
}

// kate: tab-width 4
