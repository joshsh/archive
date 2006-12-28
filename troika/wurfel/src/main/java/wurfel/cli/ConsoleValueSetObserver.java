package wurfel.cli;

import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.ObservableValueSet;

import org.openrdf.model.Resource;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.Observer;
import java.util.Observable;

public class ConsoleValueSetObserver implements Observer
{
    private ObservableValueSet valueSet;

    public ConsoleValueSetObserver( ObservableValueSet valueSet )
    {
        this.valueSet = valueSet;
    }

    private void refreshView()
        throws WurfelException
    {
        Context context = valueSet.getContext();
        Collection<Value> values = valueSet.getValues();

        if ( 0 < values.size() )
            System.out.println( "" );

        int index = 0;
        Iterator<Value> valuesIter = values.iterator();
        while ( valuesIter.hasNext() )
        {
            System.out.print( "[" + index++ + "] " );
            Value v = valuesIter.next();

            if ( v instanceof Resource )
                context.show( (Resource) v );
            else
                System.out.println( v.toString() );
        }

        System.out.println( "" );
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
