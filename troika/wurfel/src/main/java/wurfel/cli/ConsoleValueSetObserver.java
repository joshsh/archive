package wurfel.cli;

import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.EvaluationContext;
import wurfel.model.Lexicon;
import wurfel.model.ObservableValueSet;
import wurfel.model.WurfelPrintStream;

import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.Observer;
import java.util.Observable;
import java.util.Set;

public class ConsoleValueSetObserver implements Observer
{
    private ObservableValueSet valueSet;
    private WurfelPrintStream ps;

    private Lexicon lexicon;

    public ConsoleValueSetObserver( ObservableValueSet valueSet, Lexicon lexicon, EvaluationContext evalContext )
        throws WurfelException
    {
        this.valueSet = valueSet;
        this.lexicon = lexicon;
        ps = new WurfelPrintStream( System.out, lexicon, evalContext );
    }

    private void show( Resource subject, EvaluationContext evalContext )
        throws WurfelException
    {
        ps.print( subject );
        ps.print( "\n" );

        Set<URI> predicates = evalContext.getPredicates( subject );
        Iterator<URI> predIter = predicates.iterator();
        while ( predIter.hasNext() )
        {
            URI predicate = predIter.next();

            ps.print( "    " );
            ps.print( predicate );
            ps.print( "\n" );

            Set<Value> objects = valueSet.getContext().multiply( subject, predicate, evalContext );
            Iterator<Value> objIter = objects.iterator();
            while ( objIter.hasNext() )
            {
                Value object = objIter.next();

                ps.print( "        " );
                ps.print( object );
                ps.print( "\n" );
            }
        }
    }

    private void refresh()
        throws WurfelException
    {
        Context context = valueSet.getContext();
        Collection<Value> values = valueSet.getValues();

        if ( 0 < values.size() )
            ps.println( "" );

        EvaluationContext evalContext = new EvaluationContext( context, "for ConsoleValueSetObserver refresh()" );
        try
        {
            int index = 0;
            Iterator<Value> valuesIter = values.iterator();
            while ( valuesIter.hasNext() )
            {
                ps.print( "[" + index++ + "] " );
                Value v = valuesIter.next();

                if ( v instanceof Resource )
                {
                    show( (Resource) v, evalContext );
                }

                else
                {
                    ps.print( v );
                    ps.print( "\n" );
                }
            }
        }

        catch ( WurfelException e )
        {
            evalContext.close();
            throw e;
        }

        evalContext.close();

        ps.print( "\n" );
    }

    ////////////////////////////////////////////////////////////////////////////

    private boolean changed = false;
    private boolean suspended = false;

    public synchronized void suspendEventHandling()
    {
        suspended = true;
    }

    public synchronized void resumeEventHandling()
        throws WurfelException
    {
        if ( suspended )
        {
            if ( changed )
            {
                refresh();

                changed = false;
            }

            suspended = false;
        }
    }

    public void update( Observable o, Object arg )
    {
        try
        {
            if ( o == valueSet )
            {
                if ( suspended )
                    changed = true;

                else
                    refresh();
            }
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
