package net.fortytwo.ripple.model;

import java.util.Collection;
import java.util.Observable;

import org.openrdf.model.Value;

public class ObservableContainer extends Observable
{
    private Container values;
    private Model model;

    public ObservableContainer( Model model,
                                Container initialValues )
    {
        this.model = model;
        values = initialValues;
    }

    public Model getModel()
    {
        return model;
    }

    public Container getValues()
    {
        return values;
    }

    public synchronized void setValues( Container newValues )
    {
        values = newValues;
        setChanged();
        notifyObservers();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
