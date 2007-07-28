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

	final static String
		// A special "URI" for metadata about Ripple's triple store.  It is not
		// dereferenceable, and its value is specific to the particular store.
		rplStoreRootUri = "urn:net.fortytwo.ripple.store.meta",
		rplStoreSuccessMemosUri = "http://fortytwo.net/2007/03/ripple/store#successMemos",
		rplStoreFailureMemosUri = "http://fortytwo.net/2007/03/ripple/store#failureMemos";

	public static RDFFormat loadCache( final URL url,
								RDFFormat format,
								final ModelConnection mc )
		throws RippleException
	{
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

			RdfValue meta = new RdfValue( mc.createUri( rplStoreRootUri ) );
			RdfValue succ = new RdfValue( mc.createUri( rplStoreSuccessMemosUri ) );
			RdfValue fail = new RdfValue( mc.createUri( rplStoreFailureMemosUri ) );

			mc.removeStatementsAbout( meta, null );

			RdfValue rdfFirst = new RdfValue( RDF.FIRST );
			RdfValue rdfRest = new RdfValue( RDF.REST );
			RdfValue rdfNil = new RdfValue( RDF.NIL );
			RdfValue rdfType = new RdfValue( RDF.TYPE );
			RdfValue rdfList = new RdfValue( RDF.LIST );
			RdfValue last;

			last = null;
			for ( Iterator<String> iter = dereferencer.getSuccessMemos().iterator(); iter.hasNext(); )
			{
				s_logger.debug( "writing success memos" );

				// Note: apparently it's important (in Sesame 2 beta) to add an
				//       edge TO a blank node before adding edges FROM the blank
				//       node, otherwise you get a disjointed graph.  I don't
				//       know whether this is a bug or a feature.
				RdfValue cur = new RdfValue( mc.createBNode() );
				if ( null == last )
				{
					mc.add( meta, succ, cur );
					mc.add( cur, rdfType, rdfList );
				}
				else
					mc.add( last, rdfRest, cur );

				RdfValue first = mc.createValue( iter.next() );
				mc.add( cur, rdfFirst, first );

				last = cur;
			}
			if ( null != last )
				mc.add( last, rdfRest, rdfNil );

			last = null;
			for ( Iterator<String> iter = dereferencer.getFailureMemos().iterator(); iter.hasNext(); )
			{
				s_logger.debug( "writing failure memos" );

				RdfValue cur = new RdfValue( mc.createBNode() );
				if ( null == last )
				{
					mc.add( meta, fail, cur );
					mc.add( cur, rdfType, rdfList );
				}
				else
					mc.add( last, rdfRest, cur );

				RdfValue first = mc.createValue( iter.next() );
				mc.add( cur, rdfFirst, first );

				last = cur;
			}
			if ( null != last )
				mc.add( last, rdfRest, rdfNil );

			mc.close();
		}

		RdfUtils.write( model.getRepository(), out, format );
	}

	////////////////////////////////////////////////////////////////////////////

	static void updateDereferencer( final ModelConnection mc )
		throws RippleException
	{
		Model model = mc.getModel();
		Dereferencer dereferencer = model.getDereferencer();

		// Restore dereferencer state by reading success and failure memos
		// from the last session (if present).
		{
			RdfValue meta = new RdfValue( mc.createUri( rplStoreRootUri ) );
			RdfValue succ = mc.findAtMostOneObject( meta,
				new RdfValue( mc.createUri( rplStoreSuccessMemosUri ) ) );
			RdfValue fail = mc.findAtMostOneObject( meta,
				new RdfValue( mc.createUri( rplStoreFailureMemosUri ) ) );

			if ( null != succ )
			{
				s_logger.debug( "reading success memos" );
				List<RippleValue> successMemos = mc.listValue( succ );

				for ( Iterator<RippleValue> iter = successMemos.iterator(); iter.hasNext(); )
					dereferencer.addSuccessMemo( mc.stringValue( iter.next() ) );
			}

			if ( null != fail )
			{
				s_logger.debug( "reading failure memos" );
				List<RippleValue> failureMemos = mc.listValue( fail );

				for ( Iterator<RippleValue> iter = failureMemos.iterator(); iter.hasNext(); )
					dereferencer.addFailureMemo( mc.stringValue( iter.next() ) );
			}
		}
	}

}

// kate: tab-width 4
