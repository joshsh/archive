package wurfel.cli;

import wurfel.WurfelException;
import wurfel.model.ModelConnection;
import wurfel.model.Lexicon;
import wurfel.model.Model;
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

    public ConsoleValueSetObserver( ObservableValueSet valueSet, WurfelPrintStream printStream )
        throws WurfelException
    {
        this.valueSet = valueSet;
        this.lexicon = lexicon;
        ps = printStream;
    }

    private void show( Resource subject, ModelConnection mc )
        throws WurfelException
    {
        ps.print( subject );
        ps.print( "\n" );

        Set<URI> predicates = mc.getPredicates( subject );
        Iterator<URI> predIter = predicates.iterator();
        while ( predIter.hasNext() )
        {
            URI predicate = predIter.next();

            ps.print( "    " );
            ps.print( predicate );
            ps.print( "\n" );

            Set<Value> objects = valueSet.getModel().multiply( subject, predicate, mc );
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
        Model model = valueSet.getModel();
        Collection<Value> values = valueSet.getValues();

        if ( 0 < values.size() )
            ps.println( "" );

        ModelConnection mc = new ModelConnection( model, "for ConsoleValueSetObserver refresh()" );
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
                    show( (Resource) v, mc );
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
            mc.close();
            throw e;
        }

        mc.close();

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
