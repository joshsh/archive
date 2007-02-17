package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.Connection;
import org.openrdf.util.iterator.CloseableIterator;

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

    public HttpUriDereferencer()
    {
        successMemoUris = new LinkedHashSet<String>();
        failureMemoUris = new LinkedHashSet<String>();
    }

    public void dereference( final URI uri, ModelConnection mc )
        throws WurfelException
    {
        String ns = uri.getNamespace();
        String uriStr = null;

        String memo;

        // For hash namespaces, memoize the namespace.
        if ( '#' == ns.charAt( ns.length() - 1 ) )
            memo = ns;

        // For slash namespaces, we're forced to memoize the specific URI,
        // as we don't know whether to expect to get statements only about
        // the target URI or about other URIs in the namespace as well.
        else
        {
            uriStr = uri.toString();

            // For memoization purposes, a URI with a trailing slash is no
            // different than the same URI without the slash.
            if ( '/' == uriStr.charAt( uriStr.length() - 1 ) )
                memo = uriStr.substring( 0, uriStr.length() - 1 );
            else
                memo = uriStr;
        }

        if ( successMemoUris.contains( memo )
          || failureMemoUris.contains( memo ) )
            return;

        URL url;

        try
        {
            // Request the resource by its namespace, rather than by its full
            // URI.
            // Note: for hash namespaces, this doesn't make any difference.
            if ( Wurfel.dereferenceByNamespace() )
                url = new URL( ns );

            // Request the resource at the full URI.  If the purpose of a slash
            // namespace is to serve up statements about a specific URI, then
            // this is the way to go.  However, we'll lose some documents which
            // perhaps should be using a hash namespace instead.
            else
            {
                if ( null == uriStr )
                    uriStr = uri.toString();

                url = new URL( uriStr );
            }
        }

        catch ( MalformedURLException e )
        {
            throw new WurfelException( e );
        }

        // Identify the context of the to-be-imported graph with its namespace.
        URI context = mc.createUri( ns );

        try
        {
            mc.addGraph( url, context );
        }

        catch ( WurfelException e )
        {
            failureMemoUris.add( memo );
            throw e;
        }

        successMemoUris.add( memo );

// TODO: this should probably be in a parent Dereferencer.
        if ( Wurfel.enforceImplicitProvenance() )
            filter( ns, context, mc );
    }

    private void filter( final String ns, final URI context, ModelConnection mc )
        throws WurfelException
    {
        Connection conn = mc.getConnection();

        CloseableIterator<? extends Statement> stmtIter = null;

        int count = 0;

        try
        {
            stmtIter = conn.getStatements(
                 null, null, null, context, Wurfel.useInference() );

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
                stmtIter.close();
            }

            throw new WurfelException( t );
        }

        s_logger.debug( "Removed " + count + " disallowed statement(s) from context " + ns + "." );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
