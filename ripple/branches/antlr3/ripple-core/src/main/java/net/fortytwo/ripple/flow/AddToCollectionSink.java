package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.flow.Sink;

import java.util.Collection;

/**
 * Author: josh
 * Date: Mar 11, 2008
 * Time: 8:45:29 AM
 */
public class AddToCollectionSink<T, E extends Exception> implements Sink<T, E>
{
    private Collection<T> collection;

    public AddToCollectionSink(final Collection<T> coll)
    {
        this.collection = coll;
    }

    public void put(final T t) throws E
    {
        this.collection.add(t);
    }
}
