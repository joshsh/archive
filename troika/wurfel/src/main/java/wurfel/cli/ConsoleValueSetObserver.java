package wurfel.cli;

import wurfel.WurfelException;
import wurfel.Context;
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

    public ConsoleValueSetObserver( ObservableValueSet valueSet )
    {
        this.valueSet = valueSet;
        ps = new WurfelPrintStream( System.out, valueSet.getContext().getModel() );
    }

    private void show( Resource subject )
        throws WurfelException
    {
        Model model = valueSet.getContext().getModel();
        ps.print( subject );
        ps.print( "\n" );

        Set<URI> predicates = model.getPredicates( subject );
        Iterator<URI> predIter = predicates.iterator();
        while ( predIter.hasNext() )
        {
            URI predicate = predIter.next();

            ps.print( "    " );
            ps.print( predicate );
            ps.print( "\n" );

            Set<Value> objects = model.multiply( subject, predicate );
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

    private void refreshView()
        throws WurfelException
    {
        Context context = valueSet.getContext();
        Collection<Value> values = valueSet.getValues();

        if ( 0 < values.size() )
            ps.println( "" );

        int index = 0;
        Iterator<Value> valuesIter = values.iterator();
        while ( valuesIter.hasNext() )
        {
            ps.print( "[" + index++ + "] " );
            Value v = valuesIter.next();

            if ( v instanceof Resource )
            {
                show( (Resource) v );
            }

            else
            {
                ps.print( v );
                ps.print( "\n" );
            }
        }

        ps.print( "\n" );
    }

    public void update( Observable o, Object arg )
    {
        try
        {
            if ( o == valueSet )
                refreshView();
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
