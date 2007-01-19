package wurfel.model;

import wurfel.Wurfel;
import wurfel.WurfelException;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.Connection;
import org.openrdf.repository.Repository;
import org.openrdf.util.iterator.CloseableIterator;

import jline.Completor;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.LinkedHashSet;

import java.net.URL;

public class HttpUriDereferencer implements Dereferencer
{
    private static final boolean s_enforceImplicitProvenance = true;

    private Model model;
    private Repository repository;

    private Set<String> dereferencedBaseUris;

    public HttpUriDereferencer( Model model, Repository repository )
    {
        this.model = model;
        this.repository = repository;

        dereferencedBaseUris = new LinkedHashSet<String>();
    }

    public void dereferenceSubjectUri( final URI subject )
        throws WurfelException
    {
        String ns = subject.getNamespace();

        // Note: I'm counting on the hash table implementation to use compareTo()
        //       rather than equals()
        if ( !dereferencedBaseUris.contains( ns ) )
        {
            try
            {
                dereferenceGraph( ns );
            }

            catch ( WurfelException e )
            {
                // (soft fail)
            }

            dereferencedBaseUris.add( ns );
        }
    }

    private void dereferenceGraph( final String uri )
        throws WurfelException
    {
        try
        {
            URL url = new URL( uri );
            URI contextUri = Wurfel.createUri( uri );

            model.dereferenceGraph( url, contextUri );

            if ( s_enforceImplicitProvenance )
            {
                boolean includeInferred = true;
                Connection conn = repository.getConnection();
                CloseableIterator<? extends Statement> stmtIter
                    = conn.getStatements(
                        null, null, null, contextUri, includeInferred );

                while ( stmtIter.hasNext() )
                {
                    Statement st = stmtIter.next();
                    Resource subject = st.getSubject();
                    if ( subject instanceof URI && !( (URI) subject ).getNamespace().equals( uri ) )
                        conn.remove( st );
                }

                stmtIter.close();
                conn.close();
            }
        }

        catch ( Throwable t )
        {
            if ( t instanceof WurfelException )
                throw (WurfelException) t;
            else
                throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
