package wurfel.model;

import org.openrdf.model.Graph;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.sesame.sail.StatementIterator;

import jline.Completor;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedHashSet;

public abstract class Model
{
    Graph graph;

    public abstract URI resolve( final String name );
    public abstract Completor getCompletor();

    public Model( Graph g )
    {
        graph = g;
    }

    public ValueFactory getValueFactory()
    {
        return graph.getValueFactory();
    }

    /**
     *  @return  an unordered set of results
     */
    public Set<Value> multiply( Value subject,
                                Value predicate )
    {
        Set<Value> objects = new HashSet<Value>();

        if ( subject instanceof Resource && predicate instanceof URI )
        {
            Collection c
                = graph.getStatementCollection( (Resource) subject, (URI) predicate, null );

            Iterator stmtIter = c.iterator();
            while ( stmtIter.hasNext() )
                objects.add( ( (Statement) stmtIter.next() ).getObject() );
        }

        return objects;
    }

/*
    public Set<Value> multiply( Collection<Value> subjects,
                                Collection<Value> predicates )
    {
        Iterator<Value> subjIter = subjects.iterator();
        Iterator<Value> predIter;

        // Query results have a definite order.
        Set<Value> objects = new LinkedHashSet<Value>();

        while ( subjIter.hasNext() )
        {
            Value subject = subjIter.next();
            if ( subject instanceof Resource )
            {
                predIter = predicates.iterator();

                while ( predIter.hasNext() )
                {
                    Value predicate = predIter.next();
                    if ( predicate instanceof URI )
                    {
                        Collection c
                            = graph.getStatementCollection( (Resource) subject, (URI) predicate, null );

                        Iterator stmtIter = c.iterator();
                        while ( stmtIter.hasNext() )
                            objects.add( ( (Statement) stmtIter.next() ).getObject() );
                    }

                }
            }
        }

        return objects;
    }
*/

    public Set<Resource> getSubjects()
    {
        Set<Resource> subjects = new HashSet<Resource>();

        StatementIterator stmtIter = graph.getStatements();
        while ( stmtIter.hasNext() )
            subjects.add( stmtIter.next().getSubject() );

        return subjects;
    }

    public Set<URI> getPredicates( Resource subject )
    {
        Set<URI> predicates = new HashSet<URI>();

        Collection c = graph.getStatementCollection( subject, null, null );
        Iterator stmtIter = c.iterator();
        while ( stmtIter.hasNext() )
            predicates.add( ( (Statement) stmtIter.next() ).getPredicate() );

        return predicates;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
