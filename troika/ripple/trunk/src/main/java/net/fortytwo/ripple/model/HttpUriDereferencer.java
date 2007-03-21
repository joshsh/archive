package net.fortytwo.ripple.model;

import net.fortytwo.ripple.UrlFactory;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryResult;

import jline.Completor;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.LinkedHashSet;

import java.net.MalformedURLException;
import java.net.URL;

import org.apache.log4j.Logger;

public class HttpUriDereferencer implements Dereferencer
{
    private final static Logger s_logger = Logger.getLogger( HttpUriDereferencer.class );

    private Set<String> successMemoUris;
    private Set<String> failureMemoUris;

    private UrlFactory urlFactory;

    public HttpUriDereferencer( UrlFactory urlFactory )
    {
        this.urlFactory = urlFactory;

        successMemoUris = new LinkedHashSet<String>();
        failureMemoUris = new LinkedHashSet<String>();
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

        if ( successMemoUris.contains( memo )
          || failureMemoUris.contains( memo ) )
{
//if ( successMemoUris.contains( memo ) )
//s_logger.info( "URI memo already succeeded: " + memo );
//if ( failureMemoUris.contains( memo ) )
//s_logger.info( "URI memo already failed: " + memo );
            return;
}

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

        // Identify the context of the to-be-imported graph with its namespace.
        URI context = findContext( ns, mc );

        try
        {
            mc.addGraph( url, context );
s_logger.info( "#### Added " + mc.countStatements( context ) + " statements to context " + context.toString() );
        }

        catch ( RippleException e )
        {
            mc.reset();
s_logger.info( "##### failed to dereference URI: " + uri.toString() );
            failureMemoUris.add( memo );
            throw e;
        }

        successMemoUris.add( memo );

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
System.out.println( "Removing statement: " + st.getSubject().toString() + " " + st.getPredicate().toString() + " " + st.getObject().toString() + " from context " + st.getContext().toString() );
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
        Value v = rv.getRdfValue();
        if ( v instanceof URI )
        {
            String memo = findMemo( (URI) v );

            if ( failureMemoUris.contains( memo ) )
                failureMemoUris.remove( memo );

            // If resolution previously succeeded, remove all statements about
            // the value in the appropriate context.
            if ( successMemoUris.contains( memo ) )
            {
                URI context = findContext( ( (URI) v ).getNamespace(), mc );
                mc.removeStatementsAbout( rv, context );

                successMemoUris.remove( memo );
            }
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
