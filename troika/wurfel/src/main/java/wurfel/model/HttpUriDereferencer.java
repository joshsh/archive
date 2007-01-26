package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;

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

    private Context context;

    private Set<String> allBaseUris;
    private Set<String> dereferencedBaseUris;
    private Set<String> failedBaseUris;

    public HttpUriDereferencer( Context context )
    {
        this.context = context;

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
                dereferenceGraph( ns, evalContext );
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
        CloseableIterator<? extends Statement> stmtIter = null;

        int count = 0;

        try
        {
            stmtIter = conn.getStatements(
                 null, null, null, contextUri, Wurfel.useInference() );

            while ( stmtIter.hasNext() )
            {
                Statement st = stmtIter.next();
                Resource subject = st.getSubject();
                if ( subject instanceof URI && !( (URI) subject ).getNamespace().equals( uri ) )
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

        s_logger.debug( "Removed " + count + " disallowed statement(s) from context " + uri + "." );
    }

    private void dereferenceGraph( final String uri, EvaluationContext evalContext )
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

        URI contextUri = evalContext.createUri( uri );

        context.dereferenceGraph( url, contextUri, evalContext.getConnection() );

        if ( Wurfel.enforceImplicitProvenance() )
            filter( uri, contextUri, evalContext.getConnection() );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
