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

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfImporter;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.RdfUtils;

import org.apache.log4j.Logger;

import org.openrdf.model.vocabulary.RDF;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.rio.RDFFormat;

public class CacheManager
{
	final static Logger logger = Logger.getLogger( CacheManager.class );

	static RdfValue
		rplCacheRoot,
		rplCacheSuccessMemo,
		rplCacheFailureMemo;

	static boolean initialized = false;

	static void initialize( final ModelConnection mc )
		throws RippleException
	{
		rplCacheRoot = new RdfValue( mc.createUri(
			"urn:net.fortytwo.ripple.store.meta" ) );
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
		RdfSourceAdapter adapter = new RdfSourceAdapter( importer );

		format = ( null == format )
			? RdfUtils.read( url, adapter, url.toString() )
			: RdfUtils.read( url, adapter, url.toString(), format );

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
	 *  Writes cache metadata to the triple store.
	 *  Note: for now, this metadata resides in the null context.
	 */
	static void persistCacheMetadata( final ModelConnection mc )
		throws RippleException
	{
		Dereferencer dereferencer = mc.getModel().getDereferencer();

		mc.removeStatementsAbout( rplCacheRoot, null );

		logger.debug( "writing success memos" );
		for ( Iterator<String> iter = dereferencer.getSuccessMemos().iterator(); iter.hasNext(); )
		{
			mc.add( rplCacheRoot, rplCacheSuccessMemo, mc.createValue( iter.next() ) );
		}

		logger.debug( "writing failure memos" );
		for ( Iterator<String> iter = dereferencer.getFailureMemos().iterator(); iter.hasNext(); )
		{
			mc.add( rplCacheRoot, rplCacheFailureMemo, mc.createValue( iter.next() ) );
		}
	}

	/**
	 *  Restores dereferencer state by reading success and failure memos
	 *  from the last session (if present).
	 */
	static void restoreCacheMetaData( final ModelConnection mc )
		throws RippleException
	{
		Dereferencer dereferencer = mc.getModel().getDereferencer();

		Iterator<RdfValue> succIter = mc.findObjects( rplCacheRoot, rplCacheSuccessMemo ).iterator();
		while ( succIter.hasNext() )
		{
			dereferencer.addSuccessMemo( mc.stringValue( succIter.next() ) );
		}

		Iterator<RdfValue> failIter = mc.findObjects( rplCacheRoot, rplCacheFailureMemo ).iterator();
		while ( failIter.hasNext() )
		{
			dereferencer.addFailureMemo( mc.stringValue( failIter.next() ) );
		}
	}
}

// kate: tab-width 4
