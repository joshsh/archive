/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import java.net.URL;

import java.util.Collection;
import java.util.Set;
import java.util.HashSet;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.rdf.SingleContextPipe;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.StringUtils;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;

// Note: throughout this implementation, both the caching context of a URI and
//       its associated web location are the same as its success or failure 'memo'.
public class HttpUriDereferencer implements Dereferencer
{
	private static final Logger LOGGER = Logger.getLogger( HttpUriDereferencer.class );

	private static final String[] BADEXT = {"123", "3dm", "3dmf", "3gp", "8bi", "aac", "ai", "aif", "app", "asf", "asp", "asx", "avi", "bat", "bin", "bmp", "c", "cab", "cfg", "cgi", "com", "cpl", "cpp", "css", "csv", "dat", "db", "dll", "dmg", "dmp", "doc", "drv", "drw", "dxf", "eps", "exe", "fnt", "fon", "gif", "gz", "h", "hqx", "htm", "html", "iff", "indd", "ini", "iso", "java", "jpeg", "jpg", "js", "jsp", "key", "log", "m3u", "mdb", "mid", "midi", "mim", "mng", "mov", "mp3", "mp4", "mpa", "mpg", "msg", "msi", "otf", "pct", "pdf", "php", "pif", "pkg", "pl", "plugin", "png", "pps", "ppt", "ps", "psd", "psp", "qt", "qxd", "qxp", "ra", "ram", "rar", "reg", "rm", "rtf", "sea", "sit", "sitx", "sql", "svg", "swf", "sys", "tar", "tif", "ttf", "uue", "vb", "vcd", "wav", "wks", "wma", "wmv", "wpd", "wps", "ws", "xhtml", "xll", "xls", "yps", "zip"};

	private Set<String> successMemos;
	private Set<String> failureMemos;

	private Set<String> badExtensions;

	private UrlFactory urlFactory;
	private ValueFactory valueFactory;

	public HttpUriDereferencer( final UrlFactory urlFactory, final ValueFactory valueFactory )
	{
		this.urlFactory = urlFactory;
		this.valueFactory = valueFactory;

		successMemos = new HashSet<String>();
		failureMemos = new HashSet<String>();

		badExtensions = new HashSet<String>();

		for ( int i = 0; i < BADEXT.length; i++ )
		{
			badExtensions.add( BADEXT[i] );
		}
	}

	private static boolean isHttpMemo( final String memo )
	{
		return memo.startsWith( "http://" );
	}

// FIXME: two threads may dereference the same URI twice if the two calls of this method occur close enough together in time.
	public void dereference( final URI uri, final RdfSink adderSink )
		throws RippleException
	{
		final String memo = RdfUtils.inferContext( uri );

		// Don't dereference a URI that is not an HTTP URI (including file: URIs,
		// which pose a security concern).
		// Don't dereference a URI which we've already dereferenced.
		if ( !isHttpMemo( memo )
				|| successMemos.contains( memo )
				|| failureMemos.contains( memo ) )
		{
			return;
		}

		// Don't dereference a URI which appears to point to a file which is not
		// an RDF document.
		int l = memo.lastIndexOf( '.' );
		if ( l > -1 && badExtensions.contains( memo.substring( l + 1 ) ) )
		{
			return;
		}

		// Note: this URL should be treated as a "black box" once created; it
		// need not resemble the URI it was created from.
		URL url = urlFactory.createUrl( memo );

		// Note: any pre-existing context information is discarded.
		final SesameInputAdapter sa = new SesameInputAdapter(
			new SingleContextPipe( adderSink, valueFactory.createURI( memo ), valueFactory ) );

		// Attempt to import the information resource.  The web location
		// 'memo' is used as the base URI for any relative references.
		try
		{
			LOGGER.info( "Dereferencing URI <"
				+ StringUtils.escapeUriString( uri.toString() )
				+ "> at location " + url );

			RdfUtils.read( url, sa, memo );
		}

		// Exceptions encountered here are logged (withoug a stack trace) but
		// tolerated.  They usually result from missing or badly-formatted
		// Semantic Web documents.
		catch ( RippleException e )
		{
			LOGGER.info( "Failed to dereference URI <"
				+ StringUtils.escapeUriString( uri.toString() ) + ">: " + e );

			// For now, any exception thrown during the importing process
			// results in the URI being blacklisted as not dereferenceable.
			failureMemos.add( memo );

			return;
		}

		// At this point, the URI is considered to have been successfully
		// dereferenced.
		successMemos.add( memo );

// TODO: this should probably be in a parent Dereferencer.
// 		if ( Ripple.rejectNonAssociatedStatements() )
// 		{
// 			filter( uri.getNamespace(), mc.createUri( memo ), mc );
// 		}
	}

	void filter( final String ns, final URI context, final ModelConnection mc )
		throws RippleException
	{
/* TODO
		RepositoryConnection conn = mc.getRepositoryConnection();

		RepositoryResult<Statement> stmtIter = null;

		int count = 0;

		try
		{
			stmtIter = conn.getStatements(
				null, null, null, Ripple.useInference() );

			while ( stmtIter.hasNext() )
			{
				Statement st = stmtIter.next();
				Resource subject = st.getSubject();

				if ( subject instanceof URI && !( (URI) subject ).getNamespace().equals( ns ) )
				{
					conn.remove( st );
					count++;
				}
			}

			stmtIter.close();
		}

		catch ( Throwable t )
		{
			if ( null != stmtIter )
			{
				try
				{
					stmtIter.close();
				}

				catch ( Throwable t2 )
				{
					t = t2;
				}
			}

			throw new RippleException( t );
		}

		LOGGER.info( "Removed " + count + " disallowed statement(s) from context " + ns + "." );
*/
	}

	// Caution: since several URIs may share a memo (e.g. in a hash namespace),
	//          statements may bunch up around these URIs when one of them is
	//          subsequently dereferenced.  For instance, you may get
	//          "redundant" statements with the same subject and predicate, and
	//          distinct but equivalent blank nodes as object.
	public void forget( final RdfValue rv, final ModelConnection mc )
		throws RippleException
	{
		// Note: this removes statements in all contexts, including
		//       statements created with the graph primitives, and statements
		//       about blank nodes.
		mc.removeStatementsAbout( rv, null );

		Value v = rv.getRdfValue();

		if ( v instanceof URI )
		{
			String memo = RdfUtils.inferContext( (URI) v );

			if ( failureMemos.contains( memo ) )
			{
				failureMemos.remove( memo );
			}

			// If resolution previously succeeded, remove all statements about
			// the value in the appropriate context.
			if ( successMemos.contains( memo ) )
			{
				URI context = mc.createUri( memo );
//				mc.removeStatementsAbout( rv, context );

				successMemos.remove( memo );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void addSuccessMemo( final String memo )
	{
//System.out.println( "adding success memo: " + memo );
		successMemos.add( memo );
	}

	public void addFailureMemo( final String memo )
	{
//System.out.println( "adding failure memo: " + memo );
		failureMemos.add( memo );
	}

	public Collection<String> getSuccessMemos()
	{
		return successMemos;
	}

	public Collection<String> getFailureMemos()
	{
		return failureMemos;
	}
}

// kate: tab-width 4
