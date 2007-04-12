package net.fortytwo.ripple.model;

import java.util.Collection;
import java.util.Observable;

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

// kate: tab-width 4
