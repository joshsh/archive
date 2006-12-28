package wurfel.model;

import wurfel.Context;

import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Observable;

public class ObservableValueSet extends Observable
{
    private Collection<Value> values;
    private Context context;

    public ObservableValueSet( Context context,
                               Collection<Value> initialValues )
    {
        this.context = context;
        values = initialValues;
    }

    public Context getContext()
    {
        return context;
    }

    public Collection<Value> getValues()
    {
        return values;
    }

    public synchronized void setValues( Collection<Value> newValues )
    {
        values = newValues;
        setChanged();
        notifyObservers();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
