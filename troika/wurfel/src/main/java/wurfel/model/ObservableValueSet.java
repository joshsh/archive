package wurfel.model;

import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Observable;

public class ObservableValueSet extends Observable
{
    private Collection<Value> values;
    private Model model;

    public ObservableValueSet( Model model,
                               Collection<Value> initialValues )
    {
        this.model = model;
        values = initialValues;
    }

    public Model getModel()
    {
        return model;
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
