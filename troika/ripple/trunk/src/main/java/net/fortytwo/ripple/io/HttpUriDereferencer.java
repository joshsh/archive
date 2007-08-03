package net.fortytwo.ripple.io;

import java.net.MalformedURLException;
import java.net.URL;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.HashSet;

import jline.Completor;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfImporter;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.RdfUtils;
import net.fortytwo.ripple.util.ThreadWrapper;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryResult;

// Note: throughout this implementation, both the caching context of a URI and
//       its associated web location are the same as its success or failure 'memo'.
public class HttpUriDereferencer implements Dereferencer
{
	final static Logger s_logger = Logger.getLogger( HttpUriDereferencer.class );

	Set<String> successMemos;
	Set<String> failureMemos;

	UrlFactory urlFactory;

	public HttpUriDereferencer( UrlFactory urlFactory )
	{
		this.urlFactory = urlFactory;

		successMemos = new HashSet<String>();
		failureMemos = new HashSet<String>();
	}

	/**
	 *  @return  a String representation of the URL to be resolved
	 */
	static String findMemo( final URI uri )
	{
		String ns = uri.getNamespace();
		String memo;

		// For hash namespaces, the "racine" of the URI, followed by the hash
		// character (because it's a little quicker to leave it intact), is memoized.
		if ( '#' == ns.charAt( ns.length() - 1 ) )
			memo = ns;

		// For slash namespaces, we're forced to choose between requesting
		// the information resource at the full URI, or removing a local
		// name and treating the remainder as the URI of the information
		// resource.  Both kinds of documents are found on the Semantic Web.
		else
		{
			// Pro: saves time if multiple URIs resolve to the same information
			//      resource
			// Con: very many hash namespaces are not set up this way, and we
			//      may lose significant information
			if ( Ripple.dereferenceByNamespace() )
				memo = ns;

			// Pro: no information loss
			// Con: frequent repeated requests for the same document, resulting
			//      in wasted bandwidth and redundant statements
			else
				memo = uri.toString();
		}

		// Note: currently, many distinct memos may be equivalent as URLs, e.g.
		//           http://example.com/#
		//           http://example.com
		//           http://example.com/
		//           http://example.com///
		//           etc.
		return memo;
	}

	public void dereference( final URI uri, final ModelConnection mc )
		throws RippleException
	{
		final String memo = findMemo( uri );

		if ( successMemos.contains( memo ) || failureMemos.contains( memo ) )
			return;

		// Note: this URL should be treated as a "black box" once created; it
		// need not bear any relation to the URI it was created from.
		final URL url = urlFactory.createUrl( memo );

		RdfImporter importer = new RdfImporter( mc, mc.createUri( memo ) );
		final RdfSourceAdapter adapter = new RdfSourceAdapter( importer );

		// Attempt to import the information resource.  The web location
		// 'memo' is used as the base URI for any relative references.
		try
		{
			s_logger.info( "Dereferencing URI " + uri + " at location " + url );

			// Enclose the entire operation in the timeout wrapper, as there are various
			// pieces which are capable of hanging:
			//     urlConn.connect()
			//     urlConn.getContentType()
			new ThreadWrapper() {
				protected void run() throws RippleException
				{
					RdfUtils.read( url, adapter, memo );
				}
			}.start( Ripple.uriDereferencingTimeout() );

			s_logger.debug( "URI dereferenced without errors" );
		}

		// For now, any exception thrown during the importing process
		// results in the URI being blacklisted as not dereferenceable.
		catch ( RippleException e )
		{
			s_logger.info( "Failed to dereference URI: " + uri.toString() );
			failureMemos.add( memo );
			throw e;
		}

		// At this point, the URI is considered to have been successfully
		// dereferenced.
		successMemos.add( memo );

// TODO: this should probably be in a parent Dereferencer.
		if ( Ripple.enforceImplicitProvenance() )
			filter( uri.getNamespace(), mc.createUri( memo ), mc );
	}

	void filter( final String ns, final URI context, ModelConnection mc )
		throws RippleException
	{
		RepositoryConnection conn = mc.getRepositoryConnection();

		RepositoryResult<Statement> stmtIter = null;

		int count = 0;

		try
		{
			stmtIter = conn.getStatements(
				null, null, null, /*context,*/ Ripple.useInference() );

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

		s_logger.info( "Removed " + count + " disallowed statement(s) from context " + ns + "." );
	}

	public void dereference( RdfValue rv, ModelConnection mc )
		throws RippleException
	{
Value v = rv.getRdfValue();
if ( v instanceof URI )
	dereference( (URI) v, mc );
	}

	// Caution: since several URIs may share a memo (e.g. in a hash namespace),
	//          statements may bunch up around these URIs when one of them is
	//          subsequently dereferenced.  For instance, you may get
	//          "redundant" statements with the same subject and predicate, and
	//          distinct but equivalent blank nodes as object.
	public void forget( RdfValue rv, ModelConnection mc )
		throws RippleException
	{
		// Note: this removes statements in all contexts, including
		//       statements created with the graph primitives, and statements
		//       about blank nodes.
		mc.removeStatementsAbout( rv, null );

		Value v = rv.getRdfValue();
		if ( v instanceof URI )
		{
			String memo = findMemo( (URI) v );

			if ( failureMemos.contains( memo ) )
				failureMemos.remove( memo );

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
System.out.println( "adding success memo: " + memo );
		successMemos.add( memo );
	}

	public void addFailureMemo( final String memo )
	{
System.out.println( "adding failure memo: " + memo );
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
