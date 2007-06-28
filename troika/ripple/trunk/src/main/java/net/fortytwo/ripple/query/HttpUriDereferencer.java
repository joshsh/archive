package net.fortytwo.ripple.query;

import java.net.MalformedURLException;
import java.net.URL;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.LinkedHashSet;

import jline.Completor;

import net.fortytwo.ripple.util.UrlFactory;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryResult;

public class HttpUriDereferencer implements Dereferencer
{
	private final static Logger s_logger = Logger.getLogger( HttpUriDereferencer.class );

	private Set<String> successMemos;
	private Set<String> failureMemos;

	private UrlFactory urlFactory;

	public HttpUriDereferencer( UrlFactory urlFactory )
	{
		this.urlFactory = urlFactory;

		successMemos = new LinkedHashSet<String>();
		failureMemos = new LinkedHashSet<String>();
	}

	private String findMemo( final URI uri )
	{
		String ns = uri.getNamespace();

		// For hash namespaces, the namespace is memoized.
		if ( '#' == ns.charAt( ns.length() - 1 ) )
			return ns;

		// For slash namespaces, we're forced to memoize the specific URI,
		// as we don't know whether to expect to get statements only about
		// the target URI or about other URIs in the namespace as well.
		else
		{
			String uriStr = uri.toString();

			// For memoization purposes, a URI with a trailing slash is no
			// different than the same URI without the slash.
			if ( '/' == uriStr.charAt( uriStr.length() - 1 ) )
				return uriStr.substring( 0, uriStr.length() - 1 );
			else
				return uriStr;
		}
	}

	private URI findContext( final String ns, ModelConnection mc )
		throws RippleException
	{
		return mc.createUri( ns );
	}

	public void dereference( final URI uri, ModelConnection mc )
		throws RippleException
	{
		String ns = uri.getNamespace();
		String memo = findMemo( uri );

		if ( successMemos.contains( memo )
		|| failureMemos.contains( memo ) )
{
//if ( successMemos.contains( memo ) )
//s_logger.info( "URI memo already succeeded: " + memo );
//if ( failureMemos.contains( memo ) )
//s_logger.info( "URI memo already failed: " + memo );
			return;
}

		// Identify the context of the to-be-imported graph with its namespace.
		URI context = findContext( ns, mc );

		try
		{
			// Note: this URL should be treated as a "black box" once created; it
			// need not bear any relation to the URI it was created from.
			URL url;

			// Request the resource by its namespace, rather than by its full
			// URI.
			// Note: for hash namespaces, this doesn't make any difference.
			if ( Ripple.dereferenceByNamespace() )
				url = urlFactory.createUrl( ns );

			// Request the resource at the full URI.  If the purpose of a slash
			// namespace is to serve up statements about a specific URI, then
			// this is the way to go.  However, we'll lose some documents which
			// perhaps should be using a hash namespace instead.
			else
				url = urlFactory.createUrl( uri.toString() );

			mc.addGraph( url, context );
//s_logger.info( "#### Added " + mc.countStatements( context ) + " statements to context " + context.toString() );
		}

		catch ( RippleException e )
		{
//			mc.reset( true );
			s_logger.info( "Failed to dereference URI: " + uri.toString() );
			failureMemos.add( memo );
			throw e;
		}

		successMemos.add( memo );

// TODO: this should probably be in a parent Dereferencer.
		if ( Ripple.enforceImplicitProvenance() )
			filter( ns, context, mc );
	}

	private void filter( final String ns, final URI context, ModelConnection mc )
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
				URI context = findContext( ( (URI) v ).getNamespace(), mc );
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
