package net.fortytwo.ripple.io;

import java.io.InputStream;
import java.io.OutputStream;

import java.net.URL;
import java.net.URLConnection;

	import java.util.Iterator;
	import java.util.List;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.RdfUtils;

import org.apache.log4j.Logger;

import org.openrdf.model.vocabulary.RDF;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.rio.RDFFormat;

public class CacheManager
{
	final static Logger s_logger = Logger.getLogger( CacheManager.class );

	static RdfValue
		// An indexical resource for metadata about Ripple's triple store.  It is not
		// dereferenceable, and its value is specific to the particular store.
		rplStoreRoot,
		rplStoreSuccessMemo,
		rplStoreFailureMemo;

	static boolean initialized = false;
	static void initialize( final ModelConnection mc )
		throws RippleException
	{
		rplStoreRoot = new RdfValue( mc.createUri(
			"urn:net.fortytwo.ripple.store.meta" ) );
		rplStoreSuccessMemo = new RdfValue( mc.createUri(
			"http://fortytwo.net/2007/03/ripple/store#successMemo" ) );
		rplStoreFailureMemo = new RdfValue( mc.createUri(
			"http://fortytwo.net/2007/03/ripple/store#failureMemo" ) );

		initialized = true;
	}

	public static RDFFormat loadCache( final URL url,
								RDFFormat format,
								final ModelConnection mc )
		throws RippleException
	{
		if ( !initialized )
			initialize( mc );

		format = ( null == format )
			? RdfUtils.read( url, mc.getSourceAdapter(), url.toString() )
			: RdfUtils.read( url, mc.getSourceAdapter(), url.toString(), format );

		updateDereferencer( mc );

		return format;
	}

	public static void writeCacheTo( final Model model,
									final OutputStream out,
									RDFFormat format )
		throws RippleException
	{
		// Save dereferencer state.
		{
			Dereferencer dereferencer = model.getDereferencer();

			ModelConnection mc = model.getConnection( null );
			if ( !initialized )
				initialize( mc );

			mc.removeStatementsAbout( rplStoreRoot, null );

			s_logger.debug( "writing success memos" );
			for ( Iterator<String> iter = dereferencer.getSuccessMemos().iterator(); iter.hasNext(); )
				mc.add( rplStoreRoot, rplStoreSuccessMemo, mc.createValue( iter.next() ) );

			s_logger.debug( "writing failure memos" );
			for ( Iterator<String> iter = dereferencer.getFailureMemos().iterator(); iter.hasNext(); )
				mc.add( rplStoreRoot, rplStoreFailureMemo, mc.createValue( iter.next() ) );

			mc.close();
		}

		RdfUtils.write( model.getRepository(), out, format );
	}

	////////////////////////////////////////////////////////////////////////////

	/**
	 *  Restores dereferencer state by reading success and failure memos
	 *  from the last session (if present).
	 */
	static void updateDereferencer( final ModelConnection mc )
		throws RippleException
	{
		Model model = mc.getModel();
		Dereferencer dereferencer = model.getDereferencer();

		Iterator<RdfValue> succIter = mc.findObjects( rplStoreRoot, rplStoreSuccessMemo ).iterator();
		while ( succIter.hasNext() )
			dereferencer.addSuccessMemo( mc.stringValue( succIter.next() ) );

		Iterator<RdfValue> failIter = mc.findObjects( rplStoreRoot, rplStoreSuccessMemo ).iterator();
		while ( failIter.hasNext() )
			dereferencer.addSuccessMemo( mc.stringValue( failIter.next() ) );
	}

}

// kate: tab-width 4
