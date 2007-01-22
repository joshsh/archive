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

public class HttpUriDereferencer implements Dereferencer
{
    private static final boolean s_enforceImplicitProvenance = true;

    private Model model;

    private Set<String> allBaseUris;
    private Set<String> dereferencedBaseUris;
    private Set<String> failedBaseUris;

    public HttpUriDereferencer( Model model )
    {
        this.model = model;

        allBaseUris = new LinkedHashSet<String>();
        dereferencedBaseUris = new LinkedHashSet<String>();
        failedBaseUris = new LinkedHashSet<String>();
    }

    public void dereferenceSubjectUri( final URI subject, EvaluationContext evalContext )
        throws WurfelException
    {
        String ns = subject.getNamespace();

        // Note: I'm counting on the hash table implementation to use compareTo()
        //       rather than equals()
        if ( !allBaseUris.contains( ns ) )
        {
            try
            {
                dereferenceGraph( ns, evalContext.getConnection() );
            }

            catch ( WurfelException e )
            {
                allBaseUris.add( ns );
                failedBaseUris.add( ns );
                throw e;
            }

            allBaseUris.add( ns );
            dereferencedBaseUris.add( ns );
        }
    }

    private void filter( final String uri, final URI contextUri, Connection conn )
        throws WurfelException
    {
        boolean includeInferred = true;

        CloseableIterator<? extends Statement> stmtIter = null;

        try
        {
            stmtIter = conn.getStatements(
                 null, null, null, contextUri, includeInferred );

            while ( stmtIter.hasNext() )
            {
                Statement st = stmtIter.next();
                Resource subject = st.getSubject();
                if ( subject instanceof URI && !( (URI) subject ).getNamespace().equals( uri ) )
                    conn.remove( st );
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
    }

    private void dereferenceGraph( final String uri, Connection conn )
        throws WurfelException
    {
        URL url;

        try
        {
            url = new URL( uri );
        }

        catch ( MalformedURLException e )
        {
            throw new WurfelException( e );
        }

        URI contextUri = Wurfel.createUri( uri );

        model.dereferenceGraph( url, contextUri );

        if ( s_enforceImplicitProvenance )
            filter( uri, contextUri, conn );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
