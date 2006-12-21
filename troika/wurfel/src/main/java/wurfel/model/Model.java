package wurfel.model;

import wurfel.WurfelException;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.Connection;
//import org.openrdf.sail.SailConnection;
import org.openrdf.repository.Repository;
import org.openrdf.util.iterator.CloseableIterator;
import org.openrdf.sail.SailException;

import jline.Completor;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedHashSet;

public abstract class Model
{
    protected Repository repository;
    protected Resource context;

    public abstract URI resolve( final String name ) throws WurfelException;
    public abstract Completor getCompletor() throws WurfelException;

    public Model( Repository repository, Resource context )
    {
        this.repository = repository;
        this.context = context;
    }

    public ValueFactory getValueFactory()
    {
        return repository.getValueFactory();
    }

    /**
     *  @return  an unordered set of results
     */
    public Set<Value> multiply( Value subject,
                                Value predicate )
        throws WurfelException
    {
        Set<Value> objects = new HashSet<Value>();

        if ( subject instanceof Resource && predicate instanceof URI )
        {
            try
            {
                Connection conn = repository.getConnection();
                boolean includeInferred = true;
                CloseableIterator<? extends Statement> stmtIter
                    = conn.getStatements(
//                        (Resource) subject, (URI) predicate, null, context, includeInferred );
                        (Resource) subject, (URI) predicate, null, includeInferred );
                while ( stmtIter.hasNext() )
                    objects.add( stmtIter.next().getObject() );
                stmtIter.close();
                conn.close();
            }

            catch ( SailException e )
            {
                throw new WurfelException( e );
            }
        }

        return objects;
    }

    public Set<Resource> getSubjects()
        throws WurfelException
    {
        Set<Resource> subjects = new HashSet<Resource>();

        try
        {
            Connection conn = repository.getConnection();
            boolean includeInferred = true;
            CloseableIterator<? extends Statement> stmtIter
                = conn.getStatements(
//                    null, null, null, context, includeInferred );
                    null, null, null, includeInferred );
            while ( stmtIter.hasNext() )
                subjects.add( stmtIter.next().getSubject() );
            stmtIter.close();
            conn.close();
        }

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        return subjects;
    }

    public Set<URI> getPredicates( Resource subject )
        throws WurfelException
    {
        Set<URI> predicates = new HashSet<URI>();

        try
        {
            Connection conn = repository.getConnection();
            boolean includeInferred = true;
            CloseableIterator<? extends Statement> stmtIter
                = conn.getStatements(
//                    subject, null, null, context, includeInferred );
                    subject, null, null, includeInferred );
            while ( stmtIter.hasNext() )
                predicates.add( stmtIter.next().getPredicate() );
            stmtIter.close();
            conn.close();
        }

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        return predicates;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
